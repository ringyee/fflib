#include <iostream>
#include <string>
using namespace std;

#include "detail/task_queue_impl.h"
#include "thread.h"
#include "log.h"
#include "lua/fflua.h"

using namespace ff;

class base_t
{
public:
	void dump()
	{
		printf("in %s a:%d\n", __FUNCTION__, v);
	}
	int v;
};
class foo_t: public base_t
{
public:
	foo_t(int b)
	{
		printf("in %s b:%d this=%p\n", __FUNCTION__, b, this);
	}
	~foo_t()
	{
		printf("in %s\n", __FUNCTION__);
	}
	void print(int64_t a, base_t* p) const
	{
		printf("in foo_t::print a:%lld p:%p\n", a, p);
	}

	static void dumy()
	{
		printf("in %s\n", __FUNCTION__);
	}
	int a;
};

void dumy()
{
	printf("in %s\n", __FUNCTION__);
}

void lua_reg(lua_State* ls)
{
	fflua_register_t<base_t, ctor()>(ls, "base_t")
					.def(&base_t::dump, "dump")
					.def(&base_t::v, "v");


	fflua_register_t<foo_t, ctor(int)>(ls, "foo_t", "base_t")
				.def(&foo_t::print, "print")
				.def(&foo_t::a, "a")
				.def(&foo_t::dumy, "dumy");
}
int main(int argc, char* argv[])
{

	fflua_t fflua;
	fflua.load_file("test.lua");
	fflua.reg(lua_reg);
	fflua.call<base_t*>("foo", 1)->dump();


    return 0;
}
