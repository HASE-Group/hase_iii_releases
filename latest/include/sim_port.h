//---------------------------------------------------------------------------
#ifndef sim_portH
#define sim_portH
//---------------------------------------------------------------------------
typedef int sim_entity_id;

// Class sim_port
class  sim_port {
  char *pname;
  char* dest_ename;
  sim_entity_id srce, deste;
  int srci, desti;
 protected:
  int connect(sim_entity_id s);
  void set_pname(const char *pn);
  friend class entity;
 public:
  sim_port(const char* d);
  sim_port(sim_entity_id d);
  sim_port();
  ~sim_port();
  void set_dest(sim_entity_id d);
  void set_dest_ename(const char *d); // used for ARRAYPORTS initialisation
  char* get_dest_ename();
  char* get_pname();
  sim_entity_id get_dest();
  sim_entity_id get_src();
  void set_srci(int i);
  void set_desti(int i);
  int get_srci();
  int get_desti();
  static void connect(sim_port &p1, sim_port &p2);
};
#endif
