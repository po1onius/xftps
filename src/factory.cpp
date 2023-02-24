#include "factory.h"
#include "fcmd.h"
#include "fuser.h"
#include "flist.h"
#include "fport.h"
#include "fretr.h"
#include "fstore.h"
#include "fdel.h"
task *factory::CreateTask()
{
	auto *x = new fcmd();

	x->Reg("USER", new fuser());
	auto * list = new flist();
	x->Reg("PWD", list);
	x->Reg("LIST", list);
	x->Reg("CWD", list);
	x->Reg("CDUP", list);
	x->Reg("PORT", new fport());
	x->Reg("RETR", new fretr());
	x->Reg("STOR", new fstore());
    x->Reg("DELE", new fdel());
	
	return x;
}
