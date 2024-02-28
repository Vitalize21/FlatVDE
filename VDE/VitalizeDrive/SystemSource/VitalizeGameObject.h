#pragma once

#if DEVELOP_DEBUG_PC

class VirtualObject
{
public:
	VirtualObject();
	virtual ~VirtualObject();

	virtual const char* getName() = 0;
	virtual void inspectorProperty() = 0;
	
private:

};

#define VIRTUAL_OBJ : public VirtualObject
#define VIRTUAL_OBJ_MULTI , public VirtualObject

class VitalizeObjectManager
{
public:
	static void registerVirtualObject(VirtualObject* obj);
	static void unregisterVirtualObject(VirtualObject* obj);

	static std::vector<VirtualObject*>& getVirtualObjectList() { return _VirtualObjectList; }
	static VirtualObject* getObject(int id);
	static const char** getObjectNameList() { return _ObjectNameList; }
	static int getNameListSize() { return _NameListSize; }

	static void createObjectNameList();
	static void clearObjectNameList();

private:
	friend class VitalizeFramework;
	
	static void Initialize();
	static void Release();
	//static void Update();

	static std::vector<VirtualObject*> _VirtualObjectList;
	static const char** _ObjectNameList;
	static int _NameListSize;

#pragma region _Singleton
public:
	VitalizeObjectManager(const VitalizeObjectManager&) = delete;
	VitalizeObjectManager& operator=(const VitalizeObjectManager&) = delete;
	VitalizeObjectManager(VitalizeObjectManager&&) = delete;
	VitalizeObjectManager& operator=(VitalizeObjectManager&&) = delete;
private:
	VitalizeObjectManager() {}
	~VitalizeObjectManager() = default;
#pragma endregion
};

#else

//PCäJî≠ópÇ»ÇÃÇ≈è¡Ç∑
#define VIRTUAL_OBJ
#define VIRTUAL_OBJ_MULTI

#endif


