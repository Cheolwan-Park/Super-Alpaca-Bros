#include "Alpaca.hpp"
#include "AlpacaHead.hpp"
#include "AlpacaAction.hpp"

namespace Game
{
    namespace Alpaca
    {
        // ActionManager class 
        ActionManager::ActionManager()
        :m_actions(), m_lastaction(ActionManager::ActionType::NONE),  m_remain(),
        m_resettime(0.0f), m_remain_to_reset(0.0f)
        {
            ;
        }

        ActionManager::~ActionManager()
        {
            ;
        }

        void ActionManager::InitWithJson(const rapidjson::Value::Object &obj)
        {
            assert(obj.HasMember("actions"));
            assert(obj.HasMember("resettime"));
            assert(obj["actions"].IsArray());
            assert(obj["resettime"].IsFloat());

            // read actions
            auto actions = obj["actions"].GetArray();
            assert(actions.Size() <= NUM_ACTIONS);

            StackAllocator &allocator = Application::Get().GetAllocator();
            ActionFactory &factory = ActionFactory::GetGlobal();
            ActionFactory::Type factoryfunc = nullptr;
            Action *newaction = nullptr;
            const char *actiontype = nullptr;
            Uint32 actiontypeid = 0;

            for(int i=0; i<actions.Size(); ++i)
            {
                assert(actions[i].IsObject());
                auto obj = actions[i].GetObject();

                assert(obj.HasMember("type"));
                assert(obj["type"].IsString());
                actiontype = obj["type"].GetString();
                actiontypeid = ::Base::CompileTimeHash::runtime_hash(actiontype, strlen(actiontype));

                factoryfunc = factory.GetFunction(actiontypeid);
                newaction = factoryfunc(obj, allocator);
                m_actions[i] = newaction;
            }
            
            m_resettime = obj["resettime"].GetFloat();
        }

        void ActionManager::Update()
        {
            auto &t = Time::Get();
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                m_remain[i] -= t.GetDeltatime();
                if(m_actions[i])
                    m_actions[i]->Update();
            }
            
            if(!DoingAnyAction())
            {
                m_remain_to_reset -= t.GetDeltatime();
                if(m_remain_to_reset < 0.0f && m_lastaction != ActionType::NONE)
                {
                    m_lastaction = ActionType::NONE;
                }
            }
        }

        Action *ActionManager::GetAction(ActionManager::ActionType type) 
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i] && type == m_actions[i]->GetType())
                    return m_actions[i];
            }
            return nullptr;
        }

        int32 ActionManager::isAbleToAction(ActionManager::ActionType type)
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i] && type == m_actions[i]->GetType())
                    return (m_remain[i] < 0.0f);
            }
            return false;
        }

        int32 ActionManager::isAbleToAction(Uint32 idx)
        {
            assert(idx < NUM_ACTIONS);
            return (m_remain[idx] < 0.0f);
        }

        int32 ActionManager::isActing(ActionManager::ActionType type)
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i] && type == m_actions[i]->GetType())
                    return m_actions[i]->isActing();
            }
            return false;
        }

        int32 ActionManager::isActing(Uint32 idx)
        {
            assert(idx < NUM_ACTIONS);
            if(m_actions[idx])
                return m_actions[idx]->isActing();
            return false;
        }

        int32 ActionManager::DoingAnyAction()
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i] && m_actions[i]->isActing())
                    return true;
            }
            return false;
        }

        void ActionManager::DoAction(ActionManager::ActionType type)
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i] && type == m_actions[i]->GetType())
                {
                    if(m_remain[i] < 0.0f)
                    {
                        m_actions[i]->Act();
                        m_remain[i] = m_actions[i]->GetCooltime();
                        m_lastaction = type;
                    }
                }
            }
        }

        void ActionManager::DoAction(Uint32 idx)
        {
            assert(idx < NUM_ACTIONS);
            if(m_actions[idx] && m_remain[idx] < 0.0f)
            {
                m_actions[idx]->Act();
                m_remain[idx] = m_actions[idx]->GetCooltime();
                m_lastaction = m_actions[idx]->GetType();
            }
        }

        void ActionManager::CountResetTime()
        {
            m_remain_to_reset = m_resettime;
        }

        ActionManager::ActionType ActionManager::GetLastAction()const
        {
            return m_lastaction;
        }

        void ActionManager::SetAlpaca(Alpaca *alpaca)
        {
            for(int i=0; i<NUM_ACTIONS; ++i)
            {
                if(m_actions[i])
                    m_actions[i]->SetAlpaca(alpaca);
            }
        }


        // Alpaca class
        const SDL_Scancode Alpaca::keymap[2][7] = 
        {
            {
                SDL_SCANCODE_W,         // up
                SDL_SCANCODE_A,         // left
                SDL_SCANCODE_S,         // down
                SDL_SCANCODE_D,         // right
                SDL_SCANCODE_1,         // heading
                SDL_SCANCODE_2,         // spit
                SDL_SCANCODE_3          // dash
            },
            {
                SDL_SCANCODE_I,         // up
                SDL_SCANCODE_J,         // left
                SDL_SCANCODE_K,         // down
                SDL_SCANCODE_L,         // right
                SDL_SCANCODE_8,         // heading
                SDL_SCANCODE_9,         // spit
                SDL_SCANCODE_0          // dash
            }
        };

        Alpaca::Alpaca()
        :Component(), m_head(nullptr), m_headobject(), 
        m_animator(nullptr), m_rigidbody(nullptr), m_nowground(nullptr), 
        m_anims(), m_speed(0.0f), m_jumppower(0.0f), m_keymap(Keymap::NONE), 
        m_actionm()
        {
            ;
        }

        Alpaca::~Alpaca()
        {
            ;
        }

        void Alpaca::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Component::InitWithJson(obj, allocator);

            assert(obj.HasMember("idleanim"));
            assert(obj.HasMember("walkinganim"));
            assert(obj.HasMember("head"));
            assert(obj.HasMember("speed"));
            assert(obj.HasMember("jumppower"));
            assert(obj.HasMember("action_manager"));
            assert(obj["idleanim"].IsString());
            assert(obj["walkinganim"].IsString());
            assert(obj["head"].IsObject());
            assert(obj["speed"].IsFloat());
            assert(obj["jumppower"].IsFloat());
            assert(obj["action_manager"].IsObject());
            
            const char *idlestr = obj["idleanim"].GetString();
            const char *walkingstr = obj["walkinganim"].GetString();
            m_anims[0] = CompileTimeHash::runtime_hash(idlestr, strlen(idlestr));
            m_anims[1] = CompileTimeHash::runtime_hash(walkingstr, strlen(walkingstr));

            m_speed = obj["speed"].GetFloat();
            m_jumppower = obj["jumppower"].GetFloat();

            Scene *scene = Application::Get().GetScene();
            GameObject *head = scene->CreateGameObject(obj["head"].GetObject());
            head->SetID((GetGameObject()->GetID()) + "head"_hash);
            m_head = head->GetComponent<Head>();
            m_head->SetAlpaca(this);

            m_actionm.InitWithJson(obj["action_manager"].GetObject());
            m_actionm.SetAlpaca(this);
        }

        void Alpaca::Start()
        {
            Component::Start();
            GameObject *gameobjcet = GetGameObject();
            m_animator = gameobjcet->GetComponent<AnimatedSprite>();
            m_rigidbody = gameobjcet->GetComponent<Rigidbody>();
            m_hitgauge = gameobjcet->GetComponent<HitGauge>();
        }

        void Alpaca::Update()
        {
            ControlledMove();
            CheckActions();
            m_actionm.Update();
            m_headobject.Update();
        }

        void Alpaca::Release()
        {
            m_headobject.Release();
        }

        void Alpaca::OnTriggerStay(Collider *other)
        {
            if(other->GetTag() == "ground"_hash)
            {
                m_nowground = other->GetGameObject()->GetComponent<Ground>();
                SetGrounded(true);
            }
        }

        void Alpaca::OnTriggerExit(Collider *other)
        {
            if(other->GetTag() == "ground"_hash)
            {
                m_nowground = nullptr;
                SetGrounded(false);
            }
        }

        Head *Alpaca::GetHeadObject()
        {
            return m_head;
        }

        float32 Alpaca::GetSpeed()const
        {
            return m_speed;
        }

        float32 Alpaca::GetJumpPower()const
        {
            return m_jumppower;
        }

        Alpaca::Keymap Alpaca::GetKeymap()const
        {
            return m_keymap;
        }

        int32 Alpaca::isMoving()const
        {
            return m_flags.GetFlag(13);
        }

        int32 Alpaca::isGrounded()const
        {
            return m_flags.GetFlag(14);
        }

        Rigidbody *Alpaca::GetRigidbody()
        {
            return m_rigidbody;
        }

        HitGauge *Alpaca::GetHitGauge()
        {
            return m_hitgauge;
        }

        ActionManager &Alpaca::GetActionManager()
        {
            return m_actionm;
        }

        void Alpaca::SetHeadObject(Head *head)
        {
            m_head = head;
        }

        void Alpaca::SetSpeed(float32 speed)
        {
            m_speed = speed;
        }

        void Alpaca::SetKeymap(Alpaca::Keymap keymap)
        {
            m_keymap = keymap;
        }

        void Alpaca::ControlledMove()
        {
            auto &input = SDL::Input::Get();
            auto &t = Time::Get();
            const SDL_Scancode *keys = keymap[(int32)m_keymap];
            glm::vec3 delta = {0.0f, 0.0f, 0.0f};
            
            int32 hasMoved = isMoving();
            SetMoving(false);
            if(!m_actionm.DoingAnyAction())
            {
                const glm::vec2 &scale = GetScale();
                if(input.isKeyPressed(keys[(int32)Key::UP]) 
                && isGrounded()
                && (m_rigidbody->GetVelocity().y) <= 0.0f)
                {
                    m_rigidbody->AddForce(0.0f, m_jumppower, 0.0f);
                }
                if(input.isKeyPressed(keys[(int32)Key::DOWN]) && isGrounded()
                && m_nowground)
                {
                    m_nowground->Pass(this->GetGameObject());
                    SetGrounded(false);
                }
                if(input.isKeyDown(keys[(int32)Key::LEFT]))
                {
                    SetScale(Math::Abs(scale.x), scale.y);
                    delta.x -= m_speed;
                    SetMoving(true);
                }
                if(input.isKeyDown(keys[(int32)Key::RIGHT]))
                {
                    SetScale(-Math::Abs(scale.x), scale.y);
                    delta.x += m_speed;
                    SetMoving(true);
                }
            }
            
            if(isMoving() != hasMoved)
            {
                auto &animations = Application::Get().GetAnimationStorage();
                if(isMoving())
                    m_animator->SetAnimation(animations[m_anims[1]]);
                else
                    m_animator->SetAnimation(animations[m_anims[0]]);
            }

            delta *= t.GetDeltatime();
            Move(delta);
        }

        void Alpaca::CheckActions()
        {
            auto &input = SDL::Input::Get();
            const SDL_Scancode *keys = keymap[(int32)m_keymap];
            if(input.isKeyDown(keys[(int32)Key::ACTION1]))
            {
                m_actionm.DoAction(0);
            }
            if(input.isKeyDown(keys[(int32)Key::ACTION2]))
            {
                m_actionm.DoAction(1);
            }
            if(input.isKeyDown(keys[(int32)Key::ACTION3]))
            {
                m_actionm.DoAction(2);
            }
        }

        void Alpaca::SetMoving(int32 val)
        {
            m_flags.SetFlag(13, val);
        }

        void Alpaca::SetGrounded(int32 val)
        {
            m_flags.SetFlag(14, val);
        }
    }
}