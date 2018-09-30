#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <dlfcn.h>

#include <errno.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <system_error>

template <class interface, typename ...A>
class Plugin : public interface 
{
public:

    typedef interface* (*factory_t)(A...);
    typedef std::map<std::string, factory_t> plugin_map_t;

    virtual ~Plugin() {};

    static interface *getPlugin(std::string name, A...args);

    static void regPlugin(std::string &name, factory_t factory);

protected:
    Plugin() : interface() {}; 

private:
    static  plugin_map_t plugin_map;
};


template <class interface,typename ...A>
void Plugin<interface,A...>::regPlugin(std::string &name, factory_t factory)
{
    plugin_map.insert(std::make_pair(name,factory));
}

template <class interface, typename ...A>
interface *Plugin<interface, A...>::getPlugin(std::string name, A...args)
{
    typename Plugin<interface, A...>::plugin_map_t::iterator it;
    it = plugin_map.find(name);
    if (it != plugin_map.end())
        return (it->second)(args...);

    // Try once to load/find it.
    // XXX: remote "./"
    std::stringstream libname;
    libname << "./" << "lib" << name << ".so";
    void *lib = dlopen(libname.str().c_str(), RTLD_NOW);
    if (lib == NULL)
        throw std::runtime_error(dlerror());

    it = plugin_map.find(name);

    // XXX: bad implementation of plugin.  Throw
    if (it == plugin_map.end())
        throw std::runtime_error("Plugin failed to load factory");

    return (it->second)(args...);
}

template<std::string *name, class interface, class T, typename ...A>
class StaticReg {
    public:
        static interface* factory(A...args) {
            return new T(args...);
        }
        StaticReg() {
            Plugin<interface,A...>::regPlugin(*name, factory);
        }
};

template <std::string *name, class interface, class T, typename ...A>
class RegisterPlugin : public Plugin<interface,A...> {
    public:
        static StaticReg<name, interface, T, A...> regobj;
        RegisterPlugin() { }
};

template <std::string *name, class interface, class T, typename ...A>
    StaticReg<name,interface,T,A...>
        RegisterPlugin<name,interface,T,A...>::regobj;


/* And the parts used by clients.  XXX: Document at the top */
#define PLUGIN(interface,name)                                  \
std::string _ ## name ## Str __attribute__((init_priority(101))) ( #name ); \
class name : public RegisterPlugin<& _ ## name ## Str, interface, name>

#define PLUGIN_DEFS(interface, name, ...)                         \
template class RegisterPlugin<& _ ## name ## Str, interface, name, ##__VA_ARGS__>;

template <class interface, typename ...A>
typename Plugin<interface, A...>::plugin_map_t Plugin<interface, A...>::plugin_map;

#endif