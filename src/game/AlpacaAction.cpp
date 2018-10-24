#include "AlpacaAction.hpp"
#include "AlpacaHead.hpp"
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

        void Action::InitWithJson(const rapidjson::Value::Object &obj)
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

        void Heading::InitWithJson(const rapidjson::Value::Object &obj)
        {
            Action::InitWithJson(obj);
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
                    alpaca->GetGameObject()->SetWorldPosition(nowalpaca);
                }

                // end heading
                if(m_elapsedtime > m_time)
                {
                    if(isHeadHooking())
                    {
                        SetHooking(false);
                        alpaca->GetActionManager().EndAction(ActionManager::ActionType::DASH);
                    }
                    SetHeading(false);
                    head->ResetPosition();
                    alpaca->GetActionManager().EndAction(ActionManager::ActionType::HEADING);
                }
            }
        }

        void Heading::Act()
        {
            Head *head = GetAlpaca()->GetHeadObject();
            m_headingtargetpos = head->GetNeckPos();
            m_headingtargetpos.x -= m_length;
            head->GetGameObject()->SetRotation(Math::Constant::PIf/2.0f);
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
                GameObject *alpacaobject = alpaca->GetGameObject();

                glm::mat3x3 model(1.0f);
                model = glm::rotate(model, alpacaobject->GetRotation());
                model = glm::scale(model, alpacaobject->GetScale());
                head->GetGameObject()->GetWorldPosition(&m_hookingtargetpos);
                m_hookingtargetpos -= model * (head->GetNeckPos());

                m_hookingstartpos = m_hookingtargetpos;
                glm::vec3 headinglen(m_length, 0.0f, 1.0f);
                m_hookingstartpos += model * headinglen;
                
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
        :Action(), m_time(0.0f), m_length(0.0f), m_elapsedtime(0.0f), 
        m_taragetpos(), m_startpos(), m_flags()
        {
            ;
        }

        Dash::Dash(const Dash &other)
        :Action(other), m_time(other.m_time), m_length(other.m_length), m_elapsedtime(0.0f), 
        m_taragetpos(), m_startpos(), m_flags()
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
            this->m_time = other.m_time;
            this->m_length = other.m_length;
            return (*this);
        }

        void Dash::InitWithJson(const rapidjson::Value::Object &obj)
        {
            Action::InitWithJson(obj);

            assert(obj.HasMember("time"));
            assert(obj.HasMember("length"));
            assert(obj["time"].IsFloat());
            assert(obj["length"].IsFloat());

            m_time = obj["time"].GetFloat();
            m_length = obj["length"].GetFloat();
        }

        void Dash::Update()
        {
            if(isDashing())
            {
                auto &t = Time::Get();
                m_elapsedtime += t.GetDeltatime();
                
                float32 a = m_elapsedtime/m_time;
                Alpaca *alpaca = GetAlpaca();
                alpaca->GetGameObject()->SetLocalPosition(glm::mix(m_startpos, m_taragetpos, a));

                if(m_elapsedtime > m_time)
                {
                    SetDashing(false);
                    alpaca->GetActionManager().EndAction(ActionManager::ActionType::HEADING);
                }
            }
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
                GameObject *alpacaobject = alpaca->GetGameObject();
                m_startpos = alpacaobject->GetLocalPosition();
                m_taragetpos = m_startpos;
                if((alpacaobject->GetScale().x) < 0.0f)
                {
                    m_taragetpos.x += m_length;
                }
                else
                {
                    m_taragetpos.x -= m_length;
                }
                m_elapsedtime = 0.0f;
                SetDashing(true);
            }
        }

        Action::Type Dash::GetType()const
        {
            return Action::Type::DASH;
        }

        int32 Dash::isActing()const
        {
            return isDashing();
        }

        int32 Dash::isDashing()const
        {
            return m_flags.GetFlag(0);
        }

        void Dash::SetDashing(int32 val)
        {
            m_flags.SetFlag(0, val);
        }
    }
}