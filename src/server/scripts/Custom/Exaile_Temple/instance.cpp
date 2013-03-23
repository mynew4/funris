#include "ScriptPCH.h"
class instance_my_instance : public InstanceMapScript
{
    public:
    instance_my_instance() : InstanceMapScript("instance_my_instance", 600) { }
    struct instance_my_instance_InstanceScript : public InstanceScript
    {
        instance_my_instance_InstanceScript(Map* pMap) : InstanceScript(pMap) {Initialize();};
        uint64 hypnotoadDoorGUID[4];
        void Initialize()
        {
            for (uint8 i=0; i!=4; i++)
                hypnotoadDoorGUID[i]=0;
        }
        void OnGameObjectCreate(GameObject* go)
        {
            if(go && go->GetEntry()==185483)
                for (uint8 i=0; i!=4; i++)
                    if(!hypnotoadDoorGUID[i])
                    {
                        hypnotoadDoorGUID[i]=go->GetGUID();
                        return;
                    }
        }

        uint64 GetData64(uint32 identifier)
        {
            return hypnotoadDoorGUID[identifier];
        }
    };
    InstanceScript* GetInstanceScript(InstanceMap *map) const
    {
       return new instance_my_instance_InstanceScript(map);
    }
};

void AddSC_instance_my_instance()
{
    new instance_my_instance;
}
