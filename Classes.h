class RecvTable;
class RecvProp;
class ClientClass;

class RecvProp
{
public:
    char* m_pVarName;
    void* m_RecvType;
    int                     m_Flags;
    int                     m_StringBufferSize;
    int                     m_bInsideArray;
    const void* m_pExtraData;
    RecvProp* m_pArrayProp;
    void* m_ArrayLengthProxy;
    void* m_ProxyFn;
    void* m_DataTableProxyFn;
    RecvTable* m_pDataTable;
    int                     m_Offset;
    int                     m_ElementStride;
    int                     m_nElements;
    const char* m_pParentArrayPropName;
};

class RecvTable
{
public:

    RecvProp* m_pProps;
    int            m_nProps;
    void* m_pDecoder;
    char* m_pNetTableName;
    bool        m_bInitialized;
    bool        m_bInMainList;
};

class ClientClass
{
public:
    void* m_pCreateFn;
    void* m_pCreateEventFn;
    char* m_pNetworkName;
    RecvTable* m_pRecvTable;
    ClientClass* m_pNext;
    int                m_ClassID;
};

intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName)
{
    for (int i = 0; i < table->m_nProps; i++)
    {
        RecvProp prop = table->m_pProps[i];

        if (!_stricmp(prop.m_pVarName, netvarName))
        {
            return prop.m_Offset;
        }

        if (prop.m_pDataTable)
        {
            intptr_t offset = GetOffset(prop.m_pDataTable, tableName, netvarName);

            if (offset)
            {
                return offset + prop.m_Offset;
            }
        }
    }
    return 0;
}

intptr_t GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass)
{
    ClientClass* currNode = clientClass;

    for (auto currNode = clientClass; currNode; currNode = currNode->m_pNext)
    {
        if (!_stricmp(tableName, currNode->m_pRecvTable->m_pNetTableName))
        {
            return GetOffset(currNode->m_pRecvTable, tableName, netvarName);
        }
    }

    return 0;
}

enum knifeDefinitionIndex               // id
{
    WEAPON_KNIFE = 42,
    WEAPON_KNIFE_T = 59,
    WEAPON_KNIFE_BAYONET = 500,         // 0
    WEAPON_KNIFE_FLIP = 505,            // 1
    WEAPON_KNIFE_GUT = 506,             // 2
    WEAPON_KNIFE_KARAMBIT = 507,        // 3
    WEAPON_KNIFE_M9_BAYONET = 508,      // 4
    WEAPON_KNIFE_TACTICAL = 509,        // 5
    WEAPON_KNIFE_FALCHION = 512,        // 6
    WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
    WEAPON_KNIFE_BUTTERFLY = 515,       // 8
    WEAPON_KNIFE_PUSH = 516,            // 9
    WEAPON_KNIFE_URSUS = 519,           // 10
    WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
    WEAPON_KNIFE_STILETTO = 522,        // 12
    WEAPON_KNIFE_WIDOWMAKER = 523       // 13
};