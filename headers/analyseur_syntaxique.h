#ifndef __SYNT_UNITE__
#define __SYNT_UNITE__

extern int indent_xml;
extern int indent_step;

n_prog *pg();
n_l_dec *odv();
n_l_dec* ldv();
n_l_dec *ldvb();
n_dec* dv();
int ott();
n_l_dec* ldf();
n_dec* df();
n_l_dec *lp();
n_l_dec *oldv();
n_instr *i();
n_instr* iaff();
n_instr *ib();
n_l_instr* li();
n_instr* isi();
n_instr *osinon();
n_instr *itq();
n_instr *iapp();
n_instr* iret();
n_instr*iecr();
n_instr* ivide();
n_exp* Exp();
n_exp* expB();
n_exp *Conj();
n_exp *conjB();
n_exp* comp();
n_exp* compB();
n_exp* e();
n_exp *eB();
n_exp* t();
n_exp* tB();
n_exp* neg();
n_exp *f();
n_var *var();
n_exp *oind();
n_appel *appf();
n_l_exp *lexp();
n_l_exp *lexpB();
#endif
