#ifndef HADOUKEN_SCRIPTING_MODULES_COMMON_HPP
#define HADOUKEN_SCRIPTING_MODULES_COMMON_HPP

#include "../duktape.h"

#define DUK_READONLY_PROPERTY(ctx, index, name, func) \
    duk_push_string(ctx, #name); \
    duk_push_c_function(ctx, func, 0); \
    duk_def_prop(ctx, index, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);

namespace Hadouken
{
    namespace Scripting
    {
        namespace Modules
        {
            class Common
            {
            public:
                template<typename T>
                static void finalize(duk_context* ctx)
                {
                    std::string fieldName(typeid(T).name());

                    if (duk_get_prop_string(ctx, -1, std::string("\xff" + fieldName).c_str()))
                    {
                        delete static_cast<T*>(duk_get_pointer(ctx, -1));
                        duk_pop(ctx);
                    }
                }

                template<typename T>
                static T* getPointer(duk_context* ctx)
                {
                    duk_push_this(ctx);

                    T* res = 0;
                    std::string fieldName(typeid(T).name());

                    if (duk_get_prop_string(ctx, -1, std::string("\xff" + fieldName).c_str()))
                    {
                        res = static_cast<T*>(duk_get_pointer(ctx, -1));
                        duk_pop(ctx);
                    }

                    duk_pop(ctx);
                    return res;
                }

                template<typename T>
                static void setPointer(duk_context* ctx, duk_idx_t idx, T* ptr)
                {
                    std::string fieldName(typeid(T).name());

                    duk_push_pointer(ctx, ptr);
                    duk_put_prop_string(ctx, idx, std::string("\xff" + fieldName).c_str());
                }
            };
        }
    }
}

#endif
