#include <global_fns.h>
#include <cstdlib>

int pow(int base, int exp)
{
  int res=1;
  for (int i=0; i<exp; i++) 
	res *= base;
  return res;
}

t_reg_args Decode_Register(char *Input_Reg)
{
	t_reg_args new_reg;
	if (Input_Reg[0]=='R')
	  {
    	new_reg.type='R';
        new_reg.number=atoi(Input_Reg+1);
        new_reg.index=0;
          } 
	else
	  {new_reg.type='X';}
    return new_reg;
}

