#include "AlpacaAction.hpp"
#include "AlpacaHead.hpp"
#include "AlpacaSpit.hpp"
#include "GameManager.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Game
{
    namespace Alpaca
    {
        // Action class
        Action::Action()
        :m_cooltime(0.0f), m_alpaca(nullptr)
        {
            ;
        }

        Action::Action(const Action &other)
        :m_cooltime(other.m_cooltime), m_alpaca(nullptr)
        {
            ;
        }

        Action::~Action()
        {
            ;
        }

        Action &Action::operator=(const Action &other)
        {
            assert(this != &other);
            this->m_cooltime = other.m_cooltime;
            return (*this);
        }

        void Action::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            assert(obj.HasMember("cooltime"));
            assert(obj["cooltime"].IsFloat());

            m_cooltime = obj["cooltime"].GetFloat();
        }

        void Action::SetAlpaca(Alpaca *alpaca)
        {
            m_alpaca = alpaca;
        }

        Alpaca *Action::GetAlpaca()
        {
            return m_alpaca;
        }

        float32 Action::GetCooltime()const
        {
            return m_cooltime;
        }
        

        // Heading class
        Heading::Heading()
        :Action(), m_time(0.0f), m_length(0.0f), m_elapsedtime(0.0f),
        m_headingtargetpos(), m_hookingtargetpos(), m_hookingstartpos(), m_flags()
        {
            ;
        }

        Heading::Heading(const Heading &other)
        :Action(other), m_time(other.m_time), m_length(other.m_length), m_elapsedtime(0.0f),
        m_headingtargetpos(), m_hookingtargetpos(), m_hookingstartpos(), m_flags()
        {

        }

        Heading::~Heading()
        {
            ;
        }

        Heading &Heading::operator=(const Heading &other)
        {
            assert(this != &other);
            Action::operator=(other);
            this->m_time = other.m_time;
            this->m_length = other.m_length;
            return (*this);
        }

        void Heading::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Action::InitWithJson(obj, allocator);
            assert(obj.HasMember("time"));
            assert(obj.HasMember("length"));
            assert(obj["time"].IsFloat());
            assert(obj["length"].IsFloat());

            m_time = obj["time"].GetFloat();
            m_length = obj["length"].GetFloat();
        }

        void Heading::Update()
        {
            // heading
            if(isHeading())
            {
                auto &t = Time::Get();
                m_elapsedtime += t.GetDeltatime();

                float32 a = (-4.0f/(m_time*m_time))*m_elapsedtime*(m_elapsedtime-m_time);

                // heading
                Alpaca *alpaca = GetAlpaca();
                Head *head = alpaca->GetHeadObject();
                const glm::vec3 &neckpos = head->GetNeckPos();
                glm::vec3 nowhead = glm::mix(neckpos, m_headingtargetpos, a);
                head->SetHeadPos(nowhead);

                // hooking
                if(isHeadHooking())
                {
                    glm::vec3 nowalpaca = glm::mix(m_hookingtargetpos, m_hookingstartpos, a);
                    alpaca->SetWorldPosition(nowalpaca);
                }

                // end heading
                if(m_elapsedtime > m_time)
                {
                    if(isHeadHooking())
                    {
                        alpaca->GetRigidbody()->SetGravityActive(true);
                        SetHooking(false);
                    }
                    SetHeading(false);
                    head->ResetPosition();
                    head->GetGameObject()->GetComponent<CircleCollider>()->SetAvailable(false);
                    alpaca->GetActionManager().CountResetTime();
                }
            }
        }

        void Heading::Act()
        {
            Head *head = GetAlpaca()->GetHeadObject();
            m_headingtargetpos = head->GetNeckPos();
            m_headingtargetpos.x -= m_length;
            head->SetRotation(Math::Constant::PIf/2.0f);
            head->GetGameObject()->GetComponent<CircleCollider>()->SetAvailable(true);
            m_elapsedtime = 0.0f;
            SetHeading(true);
        }

        Action::Type Heading::GetType()const
        {
            return Action::Type::HEADING;
        }

        int32 Heading::isActing()const
        {
            return isHeading();
        }

        void Heading::StopHeading()
        {
            if(isHeading() && m_elapsedtime < m_time/2.0f)
            {
                m_elapsedtime = m_time - m_elapsedtime;
            }
        }

        void Heading::Hooking()
        {
            if(isHeading()) 
            {
                Alpaca *alpaca = GetAlpaca();
                Head *head = alpaca->GetHeadObject();

                glm::mat3x3 model(1.0f);
                model = glm::rotate(model, alpaca->GetRotation());
                model = glm::scale(model, alpaca->GetScale());
                head->GetWorldPosition(&m_hookingtargetpos);
                m_hookingtargetpos -= model * (head->GetNeckPos());

                m_hookingstartpos = m_hookingtargetpos;
                glm::vec3 headinglen(m_length, 0.0f, 1.0f);
                m_hookingstartpos += model * headinglen;

                alpaca->GetRigidbody()->SetGravityActive(false);
                
                StopHeading();
                SetHooking(true);
            }
        }

        int32 Heading::isHeading()const
        {
            return m_flags.GetFlag(0);
        }

        int32 Heading::isHeadHooking()const
        {
            return m_flags.GetFlag(1);
        }

        void Heading::SetHeading(int32 val)
        {
            m_flags.SetFlag(0, val);
        }

        void Heading::SetHooking(int32 val)
        {
            m_flags.SetFlag(1, val);
        }


        // Dash class
        Dash::Dash()
        :Action(), m_force(0.0f)
        {
            ;
        }

        Dash::Dash(const Dash &other)
        :Action(other), m_force(other.m_force)
        {
            ;
        }

        Dash::~Dash()
        {
            ;
        }

        Dash &Dash::operator=(const Dash &other)
        {
            assert(this != &other);
            Action::operator=(other);
            this->m_force = other.m_force;
            return (*this);
        }

        void Dash::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Action::InitWithJson(obj, allocator);

            assert(obj.HasMember("force"));
            assert(obj["force"].IsFloat());

            m_force = obj["force"].GetFloat();
        }

        void Dash::Update()
        {
            ;
        }

        void Dash::Act()
        {
            Alpaca *alpaca = GetAlpaca();
            auto &actions = alpaca->GetActionManager();
            Heading *heading = static_cast<Heading*>(actions.GetAction(Action::Type::HEADING));
            assert(heading);

            if(heading->isActing())
            {
                heading->Hooking();
            }
            else
            {
                Rigidbody *alpacarigid = alpaca->GetGameObject()->GetComponent<Rigidbody>();
                if((alpaca->GetScale().x) < 0.0f)
                {
                    alpacarigid->AddForce(m_force, 0.0f, 0.0f);
                }
                else
                {
                    alpacarigid->AddForce(-m_force, 0.0f, 0.0f);
                }
            }
            actions.CountResetTime();
        }

        Action::Type Dash::GetType()const
        {
            return Action::Type::DASH;
        }

        int32 Dash::isActing()const
        {
            return false;
        }

        
        // Spitting class
        Spitting::Spitting()
        :Action(), m_circularspits(0), m_maxspits(0), 
        m_spitobjects(nullptr), m_spits(nullptr)
        {
            ;
        }

        Spitting::~Spitting()
        {
            ;
        }

        void Spitting::InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator)
        {
            Action::InitWithJson(obj, allocator);

            assert(obj.HasMember("circularspits"));
            assert(obj.HasMember("maxspits"));
            assert(obj.HasMember("spit"));
            assert(obj["circularspits"].IsInt());
            assert(obj["maxspits"].IsInt());
            assert(obj["spit"].IsObject());

            m_circularspits = obj["circularspits"].GetInt();
            m_maxspits = obj["maxspits"].GetInt();
            auto spitjsonobj = obj["spit"].GetObject();

            m_spitobjects = allocator.Alloc<GameObject*>(m_maxspits);
            m_spits = allocator.Alloc<Spit*>(m_maxspits);
            memset(m_spitobjects, 0, sizeof(GameObject*)*m_maxspits);
            memset(m_spits, 0, sizeof(Spit*)*m_maxspits);

            Scene *scene = Application::Get().GetScene();
            for(Uint32 i=0; i<m_maxspits; ++i)
            {
                m_spitobjects[i] = scene->CreateGameObject(spitjsonobj);
                m_spits[i] = m_spitobjects[i]->GetComponent<Spit>();
                m_spitobjects[i]->SetAvailable(false);
            }
        }

        void Spitting::Update()
        {
            ;
        }

        void Spitting::Act()
        {
            Alpaca *alpaca = GetAlpaca();
            Head *alpacahead = alpaca->GetHeadObject();
            GameObject *headobject = alpacahead->GetGameObject();
            auto &actions = alpaca->GetActionManager();

            glm::vec3 pos(0.0f);
            headobject->GetWorldPosition(&pos);
            Spit *newspit = nullptr;

            // circular
            if(Action::Type::DASH == actions.GetLastAction())
            {
                float32 angle = 0.0f;
                float32 delta = 2.0f*Math::Constant::PI/(float32)m_circularspits;
                glm::vec3 dir(0.0f);
                for(Uint32 i=0; i<m_circularspits; ++i)
                {
                    dir.x = Math::Cos(angle);
                    dir.y = Math::Sin(angle);

                    newspit = GetSpit();
                    newspit->SetWorldPosition(pos);
                    newspit->Shoot(dir, alpaca);

                    angle += delta;
                }
            }
            // default
            else
            {
                newspit = GetSpit();
                newspit->SetWorldPosition(pos);

                Uint32 alpacanum = (Uint32)alpaca->GetKeymap();
                alpacanum = alpacanum ? 0 : 1;
                GameManager *gmanager = GameManager::GetGlobal();
                glm::vec3 opponentpos(0.0f);
                gmanager->GetAlpaca(alpacanum)->GetWorldPosition(&opponentpos);
                newspit->Shoot(opponentpos - pos, alpaca);    
            }
            actions.CountResetTime();
        }

        Action::Type Spitting::GetType()const
        {
            return Action::Type::SPIT;
        }

        int32 Spitting::isActing()const
        {
            return false;
        }

        Spit *Spitting::GetSpit()
        {
            for(Uint32 i=0; i<m_maxspits; ++i)
            {
                if(!(m_spitobjects[i]->isAvailable()))
                {
                    m_spitobjects[i]->SetAvailable(true);
                    return m_spits[i];
                }
            }
            fprintf(stderr, "there is no remain spit\n");
            exit(-1);
        }
    }
}