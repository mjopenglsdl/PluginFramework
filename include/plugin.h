/*
 * Plugin - C++ template library for dynamically loaded plugins
 * Copyright (C) 2009 - 2016 Douglas Kilpatrick
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author(s) may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Overview:
 * Define your interface....
 *
 * class someInterface {
 * public:
 *     virtual blah iface(blah) = 0;
 *     virtual ~someInterface() {};
 *     someInterface() {};
 * };
 *
 * In your definition, use the PLUGIN() and PLUGIN_DEFS() macros.
 * PLUGIN declares the class (inheriting from the interface, and some
 * internal stuff)  PLUGIN_DEFS declares/defines some magic registration
 * objects (and a factory)
 *
 * #include <Plugin>
 * #include <someInterface>
 *
 * PLUGIN(someIterface, someImplementation) {
 * public:
 *     virtual blah iface(blah) { whatever };
 *     ~someImplementation() {}
 * };
 * PLUGIN_DEFS(someInterface, someImplemtation);
 *
 *
 * Then in the consumer,
 *
 * #include <Plugin>
 * PLUGIN_BASE()
 *
 * somewhere at global scope.  Then....
 *
 * someInterface *i = Plugin<someInterface>::getPlugin("someImplementation");
 *
 * getPlugin will throw a runtime_error on failure.
 *
 */

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


/* The parts used by clients*/
#define TYPE_PLUGIN(interface, name)                                  \
std::string _ ## name ## Str __attribute__((init_priority(101))) ( #name ); \
class name : public RegisterPlugin<& _ ## name ## Str, interface, name>

#define DECLARE_PLUGIN(interface, name, ...) \
template class RegisterPlugin<& _ ## name ## Str, interface, name, ##__VA_ARGS__>;

template <class interface, typename ...A>
typename Plugin<interface, A...>::plugin_map_t Plugin<interface, A...>::plugin_map;


#endif
