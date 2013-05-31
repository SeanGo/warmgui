#ifndef __interface_object_h__
#define __interface_object_h__

namespace WARMGUI {

class WARMGUI_API IObject
{
public:
    IObject(void)
    {
        SetMyClass("IObject");
    }

    virtual ~IObject(void)
    {
    }

    std::string& getClass()
    {
        return _class;
    }
  
    bool isClass(const char* classname)
    {        
        return (_class.find(classname) != std::string::npos);
    }

protected:
    std::string _class;

    void SetMyClass(const char* classname)
    {
        if (!_class.empty())
            _class.append("\\");

        _class.append(classname);
    }
private:
    virtual void setClass() = 0;
};

}//namespace WARMGUI

#endif //__interface_object_h__