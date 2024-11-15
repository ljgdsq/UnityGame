#include <mono/jit/jit.h>
#include "mono/utils/mono-publib.h"
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/appdomain.h>

struct invoke_names {
    char *assm_name;
    char *name_space;
    char *name;
    char *meth_name;
};

int main()
{

    invoke_names *names=new invoke_names();
    names->assm_name = "D:\\GameEngine\\MonoDemo\\mono\\library\\MyHello2.dll";
    names->name_space = "MyHello2";
    names->name = "Class1";
    names->meth_name = "Hello";
mono_jit_set_aot_mode(MonoAotMode::MONO_AOT_MODE_FULL);
//    mono_jit_set_aot_mode(MonoAotMode::MONO_AOT_MODE_HYBRID);
    mono_set_assemblies_path("D:\\GameEngine\\MonoDemo\\mono\\library");
    mono_jit_init("");
//    mono_jit_aot_compiling();

    MonoDomain *domain = mono_domain_get ();
    MonoAssembly *assm = mono_domain_assembly_open (domain, names->assm_name);
    MonoImage *image = mono_assembly_get_image (assm);
    MonoClass *klass = mono_class_from_name (image, names->name_space, names->name);
    MonoMethod *method = mono_class_get_method_from_name (klass, names->meth_name, -1);
    MonoObject *args[] = {NULL, };
    mono_runtime_invoke (method, NULL, (void**)args, NULL);

    return 0;
}
