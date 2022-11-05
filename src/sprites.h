#include "mark.c"

extern const UINT8 sprite_positions_x[];
extern const UINT8 sprite_positions_y[];

extern void board_reset();

extern void mark_draw_pos(struct mark* selected);
extern void mark_change_pos(UINT8 button);
extern void mark_confirm_pos();

extern void setup_nought();
extern void setup_cross();

extern void setup_sprites();