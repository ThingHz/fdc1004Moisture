#ifndef DEF_h
#define DEF_h

extern void state0();
extern void state1();
extern void state2();
extern bool ZeroCallibrateSensors();

 struct flags{
  unsigned in_main_menu:1;
  unsigned scan_process:2;
  unsigned free_state:1;
  unsigned keypressed:1;
  unsigned startWaitMsg:1;
  unsigned display_Menu:1;
}genflag;

#endif DEF_h
