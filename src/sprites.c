#include <gb/gb.h>
#include "stdio.h"
#include "string.h"

#include "mark.c"

struct mark nought;
struct mark cross;

const UINT8 sprite_positions_x[3] = { 48, 80, 112 };
const UINT8 sprite_positions_y[3] = { 40, 72, 104 };

const UINT8 board_w = 2; // width & height

UBYTE current_mark = 128; // false == nought, true == cross
UINT8 board_positions[3][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

void board_reset() {
  for (UINT8 y = 0; y < 3; y++) {
    for (UINT8 x = 0; x < 3; x++) {
      board_positions[y][x] = 0;
    }
  }

  marks_reset();
}

UINT8 can_move_to_pos(INT8 x, INT8 y) {
  INT8 w = board_w;
  if (x < 0 || x > w) return 0;
  if (y < 0 || y > w) return 0;
  return !board_positions[y][x];
}

void mark_set_pos(INT8 x, INT8 y) {
  struct mark* selected;
  if (current_mark) selected = &cross;
  else selected = &nought;
  
  selected->x = x;
  selected->y = y;
}

void mark_draw_pos(struct mark* selected) { // , UINT8 x, UINT8 y

  if (selected->x == board_w + 1 && selected->y == board_w + 1) {
    for (UINT8 i = 0; i < 4; i++)
      move_sprite(selected->sprite_ids[i], 0, 0);
    return;
  }

  UINT8 x = sprite_positions_x[selected->x];
  UINT8 y = sprite_positions_y[selected->y];

  move_sprite(selected->sprite_ids[0], x, y);
  move_sprite(selected->sprite_ids[1], x + selected->width/2, y);
  move_sprite(selected->sprite_ids[2], x, y + selected->height/2);
  move_sprite(selected->sprite_ids[3], x + selected->width/2, y + selected->height/2);
}

void mark_change_pos(UINT8 J) {
  struct mark* selected;
  if (current_mark) selected = &cross;
  else selected = &nought;

  UINT8 x = selected->x;
  UINT8 y = selected->y;

  if (J == (J_LEFT | J_UP) && (x && y)) {
    for (UINT8 i = 1; i <= board_w; i++) {
      if (can_move_to_pos(x-i, y-i)) {
        mark_set_pos(x-i, y-i);
        break;

      } else if (can_move_to_pos(x-i, y-(i+1))) {
        mark_set_pos(x-i, y-(i+1));
        break;

      } else if (can_move_to_pos(x-(i+1), y-i)) {
        mark_set_pos(x-(i+1), y-i);
        break;
      }
    }

  } else if (J == (J_LEFT | J_DOWN) && (x && y <= board_w)) {
    for (UINT8 i = 1; i <= board_w; i++) {
      if (can_move_to_pos(x-i, y+i)) {
        mark_set_pos(x-i, y+i);
        break;

      } else if (can_move_to_pos(x-i, y+(i+1))) {
        mark_set_pos(x-i, y+(i+1));
        break;

      } else if (can_move_to_pos(x-(i+1), y+i)) {
        mark_set_pos(x-(i+1), y+i);
        break;
      }
    }

  } else if (J == (J_RIGHT | J_UP) && (x <= board_w && y)) {
    for (UINT8 i = 1; i <= board_w; i++) {
      if (can_move_to_pos(x+i, y-i)) {
        mark_set_pos(x+i, y-i);
        break;

      } else if (can_move_to_pos(x+i, y-(i+1))) {
        mark_set_pos(x+i, y-(i+1));
        break;

      } else if (can_move_to_pos(x+(i+1), y-i)) {
        mark_set_pos(x+(i+1), y-i);
        break;
      }
    }

  } else if (J == (J_RIGHT | J_DOWN) && (x <= board_w && y <= board_w)) {
    for (UINT8 i = 1; i <= board_w; i++) {
      if (can_move_to_pos(x+i, y+i)) {
        mark_set_pos(x+i, y+i);
        break;

      } else if (can_move_to_pos(x+i, y+(i+1))) {
        mark_set_pos(x+i, y+(i+1));
        break;

      } else if (can_move_to_pos(x+(i+1), y+i)) {
        mark_set_pos(x+(i+1), y+i);
        break;
      }
    }
    
  } else if ((J == J_LEFT) && x) {
    for (UINT8 i = 1; i <= x; i++)
      if (can_move_to_pos(x-i, y)) {
        selected->x -= i;
        break;
      }
  } else if (J & J_RIGHT && board_w - x) {
    for (UINT8 i = 1; i <= board_w - x; i++)
      if (can_move_to_pos(x+i, y)) {
        selected->x += i;
        break;
      }
  } else if (J & J_UP && y) {
    for (UINT8 i = 1; i <= y; i++)
      if (can_move_to_pos(x, y-i)) {
        selected->y -= i;
        break;
      }
  } else if (J & J_DOWN && board_w - y) {
    for (UINT8 i = 1; i <= board_w - y; i++)
      if (can_move_to_pos(x, y+i)) {
        selected->y += i;
        break;
      }
  }
  
  mark_draw_pos(selected);
}

void mark_confirm_pos() {
  struct mark* selected;
  if (current_mark) selected = &cross;
  else selected = &nought;

  UINT8 x = selected->x;
  UINT8 y = selected->y;
  UINT8 w = selected->width;
  UINT8 h = selected->height;
  UINT8 sprite_x = sprite_positions_x[x]/8;
  UINT8 sprite_y = sprite_positions_y[y]/8;


  set_bkg_tiles(
    sprite_x - 1, sprite_y - 2,
    w/8, h/8,
    selected->tiles
  );

  board_positions[y][x] = 1;

  mark_set_pos(board_w + 1, board_w + 1);
  mark_draw_pos(selected);

  current_mark += 128; // toggle between marks
  if (current_mark) selected = &cross;
  else selected = &nought;

  // find nearest free tile:
  UINT8 found[2] =  { x, y };
  for (UINT8 i = 1; i <= board_w; i++) {

    if (x + i <= board_w && !board_positions[y][x+i]) 
      found[0] = x + i;
    else if (x - i >= 0 && !board_positions[y][x-i]) 
      found[0] = x - i;
    else if (y + i <= board_w && !board_positions[y+1][x]) 
      found[1] = y + i;
    else if (y - i >= 0 && !board_positions[y-1][x]) 
      found[1] = y - i;
    
    break;
  }

  if (found[0] != x || found[1] != y) {
    mark_set_pos(found[0], found[1]);
  } else {
    for (UINT8 y = 0; y < 3; y++) {
      for (UINT8 x = 0; x < 3; x++) {
        if (can_move_to_pos(x, y))
          mark_set_pos(x, y);
      }
    }
  }

  mark_draw_pos(selected);
}

void marks_reset() {
  current_mark = 128;

  nought.x = 3;
  nought.y = 3;

  cross.x = 1;
  cross.y = 1;

  mark_draw_pos(&nought);
  mark_draw_pos(&cross);
}

void setup_nought() {
  nought.x = 3;
  nought.y = 3;
  nought.width = 16;
  nought.height = 16;

  for (size_t i = 0; i < 4; i++)
    nought.tiles[i] = 145 + i;

  for (size_t i = 0; i < 4; i++)
    nought.sprite_ids[i] = i;

  mark_draw_pos(&nought);
}

void setup_cross() {
  cross.x = 1;
  cross.y = 1;
  cross.width = 16;
  cross.height = 16;

  for (size_t i = 0; i < 4; i++)
    cross.tiles[i] = 149 + i;

  for (size_t i = 0; i < 4; i++)
    cross.sprite_ids[i] = i + 4;
  
  mark_draw_pos(&cross);
}

void setup_sprites() {
  for (size_t i = 0; i < 8; i++)
    set_sprite_tile(i, 128 + 17 + i);
}