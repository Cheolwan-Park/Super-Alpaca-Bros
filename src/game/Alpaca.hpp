#ifndef GameAlpaca_hpp
#define GameAlpaca_hpp

#include "base.hpp"

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

            void EndAction(ActionType type);

            const ActionType *GetActionStack()const;

            ActionType GetLastAction()const;

            Uint32 GetActionDidCount()const;

            void SetAlpaca(Alpaca *alpaca);

        private:
            FixedArray<Action*, NUM_ACTIONS> m_actions;
            ActionType m_stack[NUM_ACTIONS];
            float32 m_remain[NUM_ACTIONS];
            Uint32 m_count;
            float32 m_resettime;
            float32 m_remain_to_reset;
        };

        class Alpaca : public Component
        {
        private:
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

            // get
            Head *GetHeadObject();

            float32 GetSpeed()const;

            Alpaca::Keymap GetKeymap()const;

            int32 isMoving()const;

            int32 isDoingAction()const;

            ActionManager &GetActionManager();

            // set
            void SetHeadObject(Head *head);

            void SetSpeed(float32 speed);

            void SetKeymap(Alpaca::Keymap keymap);

        private:
            void ControlledMove();

            void CheckActions();

            void SetMoving(int32 val);

        private:
            Head *m_head;
            AnimatedSprite *m_animator;
            float32 m_speed;
            Keymap m_keymap;
            ActionManager m_actionm;
            /* flags
            * AnimatedSprite's flags
            * 13 : nowmoving
            */
        };
    }
}

#endif