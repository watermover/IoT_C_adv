#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
double DELAY = 0.1;
#define PLAYERS  2

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10,SEED_NUMBER=3,CONTROLS=2};

#define OBJ_SNAKE1 1
#define OBJ_SNAKE2 2
#define OBJ_FOOD   3

struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons;

struct control_buttons default_controls[CONTROLS] = {{KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
                                                    {'s', 'w', 'a', 'd'}};

typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons controls;
    int colorId;
} snake_t;

typedef struct tail_t
{
    int x;
    int y;
} tail_t;

struct food
{
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food[MAX_FOOD_SIZE];

void setColor(int objectType)
{
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    switch (objectType)
    {
        case OBJ_SNAKE1:
            attron(COLOR_PAIR(1));
            break;
        case OBJ_SNAKE2:
            attron(COLOR_PAIR(2));
            break;
        case OBJ_FOOD:
            attron(COLOR_PAIR(3));
            break;
    }
}

void initFood(struct food f[], size_t size)
{
    struct food init = {0,0,0,0,0};
    for(size_t i=0; i<size; i++)
    {
        f[i] = init;
    }
}

void putFoodSeed(struct food *fp)
{
    int max_x=0, max_y=0;
    char spoint[2] = {0};
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(fp->y, fp->x, " ");
    fp->x = rand() % (max_x - 1);
    fp->y = rand() % (max_y - 2) + 1;
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    spoint[0] = fp->point;
    setColor(OBJ_FOOD);
    mvprintw(fp->y, fp->x, "%s", spoint);
}

void putFood(struct food f[], size_t number_seeds)
{
    for(size_t i=0; i<number_seeds; i++)
    {
        putFoodSeed(&f[i]);
    }
}

void refreshFood(struct food f[], int nfood)
{
    for(size_t i=0; i<nfood; i++)
    {
        if( f[i].put_time )
        {
            if( !f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS )
            {
                putFoodSeed(&f[i]);
            }
        }
    }
}

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t= {0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}

void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head[], size_t size, int x, int y,int i)
{
    head[i]    = (snake_t*)malloc(sizeof(snake_t));
    tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head[i], x, y);
    head[i]->tail     = tail;
    head[i]->tsize    = size+1;
    //~ head[i]->controls = default_controls[i];
    head[i]->controls = default_controls[0];
    head[i]->colorId = (i == 0 ? OBJ_SNAKE1 : OBJ_SNAKE2);
}

void go(struct snake_t *head)
{
    char ch = '@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(head->y, head->x, " ");
    setColor(head->colorId);
    switch (head->direction)
    {
    case LEFT:
        if(head->x <= 0)
            head->x = max_x;
        mvprintw(head->y, --(head->x), "%c", ch);
        break;
    case RIGHT:
        mvprintw(head->y, ++(head->x), "%c", ch);
        break;
    case UP:
        mvprintw(--(head->y), head->x, "%c", ch);
        break;
    case DOWN:
        mvprintw(++(head->y), head->x, "%c", ch);
        break;
    default:
        break;
    }
    refresh();
}

void changeDirection(struct snake_t* snake, const int32_t key)
{
    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;
}

void goTail(struct snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if( head->tail[i].y || head->tail[i].x)
        {
            setColor(head->colorId);
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
        }
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

_Bool haveEat(struct snake_t *head, struct food f[])
{
    for (size_t i = 0; i < SEED_NUMBER; i++)
    {
        if (f[i].enable && f[i].x == head->x && f[i].y == head->y)
        {
            f[i].enable = 0;
            mvprintw(f[i].y, f[i].x, " ");
            return 1;
        }
    }
    return 0;
}

void addTail(struct snake_t *head)
{
    if (head->tsize >= MAX_TAIL_SIZE)
        return;

    head->tail[head->tsize] = head->tail[head->tsize - 1];
    head->tsize++;
}

int checkDirection(snake_t* snake, int32_t key)
{
    int want = 0;
    if (key == snake->controls.down)      want = DOWN;
    else if (key == snake->controls.up)   want = UP;
    else if (key == snake->controls.right)want = RIGHT;
    else if (key == snake->controls.left) want = LEFT;
    else return 0;

    if ((snake->direction == LEFT  && want == RIGHT) ||
        (snake->direction == RIGHT && want == LEFT)  ||
        (snake->direction == UP    && want == DOWN)  ||
        (snake->direction == DOWN  && want == UP))
        return 0;

    return 1;
}

void update(struct snake_t *head, struct food f[], const int32_t key)
{
    clock_t begin = clock();
    go(head);
    goTail(head);
    if (checkDirection(head,key))
    {
        changeDirection(head, key);
    }
    refreshFood(food, SEED_NUMBER);
    if (haveEat(head,food))
    {
        addTail(head);
    }
    refresh();
    while ((double)(clock() - begin)/CLOCKS_PER_SEC<DELAY)
    {}
}

_Bool isCrush(snake_t * snake)
{
    for (size_t i = 1; i < snake->tsize; i++)
    {
        if (snake->tail[i].x == snake->x && snake->tail[i].y == snake->y)
            return 1;
    }
    return 0;
}

void repairSeed(struct food f[], size_t nfood, struct snake_t *head)
{
    for( size_t i=0; i<head->tsize; i++ )
        for( size_t j=0; j<nfood; j++ )
        {
            if (f[j].enable &&
                head->tail[i].x == f[j].x &&
                head->tail[i].y == f[j].y)
                putFoodSeed(&f[j]);
        }
    for( size_t i=0; i<nfood; i++ )
        for( size_t j=0; j<nfood; j++ )
        {
            if (i != j &&
                f[i].enable && f[j].enable &&
                f[i].x == f[j].x &&
                f[i].y == f[j].y)
                putFoodSeed(&f[j]);
        }
}

int main()
{
    snake_t* snakes[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
        initSnake(snakes,START_TAIL_SIZE,10+i*10,10+i*10,i);

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);
    mvprintw(0, 0,"Use arrows for control. Press 'F10' for EXIT");
    timeout(0);

    start_color();
    init_pair(1, COLOR_RED,   COLOR_BLACK);
    init_pair(2, COLOR_BLUE,  COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER);
    int key_pressed=0;
    while( key_pressed != STOP_GAME )
    {
        key_pressed = getch();
        for (int i = 0; i < PLAYERS; i++)
        {
            update(snakes[i], food, key_pressed);
            if(isCrush(snakes[i]))
                break;
            repairSeed(food, SEED_NUMBER, snakes[i]);
        }
    }
    for (int i = 0; i < PLAYERS; i++)
    {
        free(snakes[i]->tail);
        free(snakes[i]);
    }
    endwin();
    return 0;
}
