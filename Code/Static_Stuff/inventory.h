typedef struct
{
short sx,sy;	//string pos
short lex,ley;	//line end
} spoint;

void do_inventory();
void do_inventory_single_frame();
int do_sell_kit(int _class);	//draws picture and returns - speed is not an issue.
void draw_topbottom_lines();

