#ifndef GameAlpacaAction_hpp
#define GameAlpacaAction_hpp

#include "base.hpp"
#include "Alpaca.hpp"



namespace Game 
{
    namespace Alpaca
    {
        #define ACTION(__TYPENAME__)    \
        MAKE_TYPE_ID(__TYPENAME__);         \
        virtual Uint32 GetTypeID()const { return ID; }

        class Action
        {
        public:
            typedef ActionManager::ActionType Type;
            typedef Action* (FactoryFunc)(const ::rapidjson::Value::Object&, ::Base::StackAllocator&);
        public:
            Action();

            // not copying alpaca pointer
            Action(const Action &other);

            virtual ~Action();

            // not copying alpaca pointer
            Action &operator=(const Action &other);

            virtual void InitWithJson(const rapidjson::Value::Object &obj);

            virtual void Update() = 0;

            virtual void Act() = 0;

            virtual Type GetType()const = 0;

            virtual int32 isActing()const = 0;

            virtual Uint32 GetTypeID()const = 0;

            void SetAlpaca(Alpaca *alpaca);

            Alpaca *GetAlpaca();

            float32 GetCooltime()const;

        private:
            float32 m_cooltime;
            Alpaca *m_alpaca;
        };

        template<class T>
        Action *ActionDefaultFactory(const ::rapidjson::Value::Object &obj, ::Base::StackAllocator &allocator)
        {
            Action *result = new (allocator.Alloc<T>()) T();
            assert(result);
            result->InitWithJson(obj);
            return result;
        }


        class Heading : public Action
        {
        public:
            ACTION(Heading);

            Heading();

            // copying time, length
            Heading(const Heading &other);

            virtual ~Heading();

            // copying time, length
            Heading &operator=(const Heading &other);

            virtual void InitWithJson(const rapidjson::Value::Object &obj);

            virtual void Update();

            virtual void Act();

            virtual Action::Type GetType()const;

            virtual int32 isActing()const;

            void StopHeading();

            void Hooking();

            // get
            int32 isHeading()const;

            int32 isHeadHooking()const;

        private:
            void SetHeading(int32 val);

            void SetHooking(int32 val);

        private:
            float32 m_time;
            float32 m_length;
            float32 m_elapsedtime; 
            glm::vec3 m_headingtargetpos;
            glm::vec3 m_hookingtargetpos;
            glm::vec3 m_hookingstartpos;
            BitFlag m_flags;
            /*
             * 0 : heading
             * 1 : hooking
             */
        };

        class Dash : public Action
        {
        public:
            ACTION(Dash);

            Dash();

            // copying length , time
            Dash(const Dash &other);

            virtual ~Dash();

            // copying length , time
            Dash &operator=(const Dash &other);

            virtual void InitWithJson(const rapidjson::Value::Object &obj);

            virtual void Update();

            virtual void Act();

            virtual Action::Type GetType()const;
            
            virtual int32 isActing()const;

            // get 
            int32 isDashing()const;

        private:
            void SetDashing(int32 val);

        private:
            float32 m_time;
            float32 m_length;
            float32 m_elapsedtime;
            glm::vec3 m_taragetpos;
            glm::vec3 m_startpos;
            BitFlag m_flags;
            /*
             * 0 : dashing
             */
        };



        class ActionFactory : public Storage<Action::FactoryFunc>
        {
        public:

            ActionFactory() : Storage<Action::FactoryFunc>() { ; }

            ActionFactory(const ActionFactory &other) = delete;

            ~ActionFactory() { Clear(); }

            ActionFactory &operator=(const ActionFactory) = delete;

            template <typename T>
            void AddFunction()
            {
                Register(&ActionDefaultFactory<T>, T::ID);
            }

            template <typename T>
            Action::FactoryFunc *GetFunction()
            {
                return (Get(T::ID));
            }

            Action::FactoryFunc *GetFunction(Uint32 id)
            {
                return (Get(id));
            }

            static ActionFactory &GetGlobal()
            {
                static ActionFactory instance;
                return instance;
            }
        };
    }
}


#endif