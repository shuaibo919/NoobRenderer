#pragma once
#include "Engine/NoobEngine.h"
namespace NoobRenderer
{
    namespace Application
    {
        class Selected: public Singleton<Selected>
        {
            private: 
            
            public:
                Selected() = delete;
                Selected(token){}
                Scene::Node::Ptr node{nullptr};
        };
    }
}