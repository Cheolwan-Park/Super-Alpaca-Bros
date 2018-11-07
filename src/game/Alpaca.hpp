#ifndef GameAlpaca_hpp
#define GameAlpaca_hpp

#include "base.hpp"
#include "Ground.hpp"
#include "HitGauge.hpp"

using namespace Base;

namespace Game
{
    namespace Alpaca 
    {
        class Head;
        class Action;
        class Alpaca;

        class ActionManager
        {
        public:
            static constexpr Uint32 NUM_ACTIONS = 4;
            enum class ActionType : Uint32 { NONE = 0, HEADING = 1, SPIT = 2, DASH = 3 };

        public:
            ActionManager();

            ActionManager(const ActionManager &other) = delete;

            ~ActionManager();

            ActionManager &operator=(const ActionManager &other) = delete;

            void InitWithJson(const rapidjson::Value::Object &obj);

            void Update();

            Action *GetAction(ActionType type);

            int32 isAbleToAction(ActionType type);

            int32 isAbleToAction(Uint32 idx);

            int32 isActing(ActionType type);

            int32 isActing(Uint32 idx);

            int32 DoingAnyAction();

            void DoAction(ActionType type);

            void DoAction(Uint32 idx);

            void CountResetTime();

            ActionType GetLastAction()const;

            void SetAlpaca(Alpaca *alpaca);

        private:
            FixedArray<Action*, NUM_ACTIONS> m_actions;
            ActionType m_lastaction;
            float32 m_remain[NUM_ACTIONS];
            float32 m_resettime;
            float32 m_remain_to_reset;
        };

        class Alpaca : public Component
        {
        public:
            static const SDL_Scancode keymap[2][7];
            enum class Keymap : int32 { NONE = -1, MAP1 = 0, MAP2 = 1 };
            enum class Key : int32 
            {
                UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3,
                ACTION1 = 4, ACTION2 = 5, ACTION3 = 6, ACTION4 = 7
            };
            typedef ActionManager::ActionType ActionType;

        public:
            COMPONENT(Alpaca);

            Alpaca();

            Alpaca(const Alpaca &other) = delete;

            virtual ~Alpaca();

            Alpaca &operator=(const Alpaca &other) = delete;

            virtual void InitWithJson(const rapidjson::Value::Object &obj, StackAllocator &allocator);

            virtual void Start();

            virtual void Update();

            virtual void Release();

            virtual void OnTriggerStay(Collider *other);

            virtual void OnTriggerExit(Collider *other);

            // get
            Head *GetHeadObject();

            float32 GetSpeed()const;

            float32 GetJumpPower()const;

            Alpaca::Keymap GetKeymap()const;

            int32 isMoving()const;

            int32 isGrounded()const;

            Rigidbody *GetRigidbody();

            HitGauge *GetHitGauge();

            ActionManager &GetActionManager();

            // set
            void SetHeadObject(Head *head);

            void SetSpeed(float32 speed);

            void SetKeymap(Alpaca::Keymap keymap);

        private:
            void ControlledMove();

            void CheckActions();

            void SetMoving(int32 val);

            void SetGrounded(int32 val);

        private:
            Head *m_head;
            GameObject m_headobject;
            AnimatedSprite *m_animator;
            Rigidbody *m_rigidbody;
            HitGauge *m_hitgauge;
            Ground *m_nowground;
            float32 m_speed;
            float32 m_jumppower;
            Keymap m_keymap;
            ActionManager m_actionm;
        };
    }
}

#endif