#include <fxlib.h>
#include <filebios.h>
#include <keybios.h>

/*
    Pokemon Card Database
    Casio fx-9860G / 9750G Legacy SDK

    Database file:
        \\fls0\\CARDS.DAT

    No Set
    No Card Number

    Maximum:
        50 cards
        4 attacks per card
*/


/* =========================================================
   SETTINGS
   ========================================================= */

#define MAX_CARDS   50
#define MAX_ATTACKS 4

#define NAME_LEN    32
#define TYPE_LEN    16
#define DAMAGE_LEN  8
#define ENERGY_LEN  24
#define DESC_LEN    64


/* =========================================================
   DATABASE FILE
   ========================================================= */

static FONTCHARACTER CardFile[] =
{
    '\\','\\','f','l','s','0','\\',
    'C','A','R','D','S','.','D','A','T',
    0
};


/* =========================================================
   CARD DATA
   ========================================================= */

typedef struct
{
    char name[NAME_LEN];
    char damage[DAMAGE_LEN];
    char energy[ENERGY_LEN];
    char description[DESC_LEN];

} Attack;


typedef struct
{
    char name[NAME_LEN];

    int hp;

    char type1[TYPE_LEN];
    char type2[TYPE_LEN];

    int attackCount;

    Attack attacks[MAX_ATTACKS];

    char weakness[TYPE_LEN];
    char resistance[TYPE_LEN];

    int retreat;

} Card;


/* =========================================================
   GLOBALS
   ========================================================= */

static Card CurrentCard;

static int CardCount;


/* =========================================================
   SCREEN
   ========================================================= */

static void ClearScreen(void)
{
    Bdisp_AllClr_DDVRAM();
}


static void WaitKey(void)
{
    unsigned int key;

    GetKey(&key);
}


static void Message(const char *text)
{
    ClearScreen();

    locate(1, 3);
    Print((unsigned char *)text);

    locate(1, 7);
    Print((unsigned char *)"Press EXE");

    WaitKey();
}


static void Problem(void)
{
    Message("User, we have a problem");
}


/* =========================================================
   INTEGER PRINT
   ========================================================= */

static void PrintInt(int value)
{
    char buffer[12];

    int length;
    int i;
    char temp;

    if (value == 0)
    {
        Print((unsigned char *)"0");
        return;
    }

    if (value < 0)
    {
        Print((unsigned char *)"-");
        value = -value;
    }

    length = 0;

    while (value > 0)
    {
        buffer[length] =
            (char)('0' + (value % 10));

        value = value / 10;

        length++;
    }

    buffer[length] = 0;

    for (i = 0; i < length / 2; i++)
    {
        temp = buffer[i];

        buffer[i] =
            buffer[length - i - 1];

        buffer[length - i - 1] =
            temp;
    }

    Print((unsigned char *)buffer);
}


/* =========================================================
   KEYBOARD
   ========================================================= */

static char KeyToChar(unsigned int key)
{
    switch (key)
    {
        case KEY_CHAR_0: return '0';
        case KEY_CHAR_1: return '1';
        case KEY_CHAR_2: return '2';
        case KEY_CHAR_3: return '3';
        case KEY_CHAR_4: return '4';
        case KEY_CHAR_5: return '5';
        case KEY_CHAR_6: return '6';
        case KEY_CHAR_7: return '7';
        case KEY_CHAR_8: return '8';
        case KEY_CHAR_9: return '9';

        case KEY_CHAR_A: return 'A';
        case KEY_CHAR_B: return 'B';
        case KEY_CHAR_C: return 'C';
        case KEY_CHAR_D: return 'D';
        case KEY_CHAR_E: return 'E';
        case KEY_CHAR_F: return 'F';
        case KEY_CHAR_G: return 'G';
        case KEY_CHAR_H: return 'H';
        case KEY_CHAR_I: return 'I';
        case KEY_CHAR_J: return 'J';
        case KEY_CHAR_K: return 'K';
        case KEY_CHAR_L: return 'L';
        case KEY_CHAR_M: return 'M';
        case KEY_CHAR_N: return 'N';
        case KEY_CHAR_O: return 'O';
        case KEY_CHAR_P: return 'P';
        case KEY_CHAR_Q: return 'Q';
        case KEY_CHAR_R: return 'R';
        case KEY_CHAR_S: return 'S';
        case KEY_CHAR_T: return 'T';
        case KEY_CHAR_U: return 'U';
        case KEY_CHAR_V: return 'V';
        case KEY_CHAR_W: return 'W';
        case KEY_CHAR_X: return 'X';
        case KEY_CHAR_Y: return 'Y';
        case KEY_CHAR_Z: return 'Z';

        case KEY_CHAR_SPACE: return ' ';
        case KEY_CHAR_MINUS: return '-';
        case KEY_CHAR_PLUS: return '+';
        case KEY_CHAR_DP: return '.';
        case KEY_CHAR_COMMA: return ',';
    }

    return 0;
}


/* =========================================================
   TEXT INPUT
   ========================================================= */

static int EditText(
    char *buffer,
    int maxLength,
    const char *title
)
{
    int length;

    unsigned int key;

    char c;


    length = 0;

    buffer[0] = 0;


    while (1)
    {
        ClearScreen();

        locate(1, 1);
        Print((unsigned char *)title);

        locate(1, 3);
        Print((unsigned char *)buffer);

        locate(1, 6);
        Print((unsigned char *)"DEL = Erase");

        locate(1, 7);
        Print((unsigned char *)"EXE=OK EXIT=Cancel");


        GetKey(&key);


        if (key == KEY_CTRL_EXE)
        {
            return 1;
        }


        if (key == KEY_CTRL_EXIT)
        {
            return 0;
        }


        if (key == KEY_CTRL_DEL)
        {
            if (length > 0)
            {
                length--;

                buffer[length] = 0;
            }

            continue;
        }


        c = KeyToChar(key);


        if (c != 0)
        {
            if (length < maxLength - 1)
            {
                buffer[length] = c;

                length++;

                buffer[length] = 0;
            }
        }
    }
}


/* =========================================================
   NUMBER INPUT
   ========================================================= */

static int EditNumber(
    int *value,
    const char *title
)
{
    char buffer[12];

    int i;


    buffer[0] = 0;


    if (
        !EditText(
            buffer,
            12,
            title
        )
    )
    {
        return 0;
    }


    *value = 0;


    for (i = 0; buffer[i] != 0; i++)
    {
        if (
            buffer[i] >= '0' &&
            buffer[i] <= '9'
        )
        {
            *value =
                (*value * 10) +
                (buffer[i] - '0');
        }
    }


    return 1;
}


/* =========================================================
   CLEAR CARD
   ========================================================= */

static void ClearCard(void)
{
    unsigned char *p;

    int i;


    p = (unsigned char *)&CurrentCard;


    for (
        i = 0;
        i < sizeof(Card);
        i++
    )
    {
        p[i] = 0;
    }
}


/* =========================================================
   DATABASE
   ========================================================= */


/*
    Database layout:

    int CardCount
    Card[0]
    Card[1]
    Card[2]
    ...

    The file is created large enough for MAX_CARDS.
*/


static int DatabaseExists(void)
{
    int handle;


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_READ
        );


    if (handle < 0)
    {
        return 0;
    }


    Bfile_CloseFile(handle);


    return 1;
}


/*
    Create CARDS.DAT.

    This is ONLY called when the first card
    is added and the file doesn't exist.
*/

static int CreateDatabase(void)
{
    int handle;

    int zero;


    if (
        Bfile_CreateFile(
            CardFile,
            sizeof(int) +
            (sizeof(Card) * MAX_CARDS)
        ) < 0
    )
    {
        return 0;
    }


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_WRITE
        );


    if (handle < 0)
    {
        return 0;
    }


    zero = 0;


    if (
        Bfile_WriteFile(
            handle,
            &zero,
            sizeof(int)
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    Bfile_CloseFile(handle);


    return 1;
}


/*
    Read number of cards.
*/

static int LoadCardCount(void)
{
    int handle;

    int count;


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_READ
        );


    if (handle < 0)
    {
        return 0;
    }


    if (
        Bfile_ReadFile(
            handle,
            &count,
            sizeof(int),
            0
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    Bfile_CloseFile(handle);


    if (count < 0)
    {
        return 0;
    }


    if (count > MAX_CARDS)
    {
        return 0;
    }


    return count;
}


/*
    Save number of cards.
*/

static int SaveCardCount(int count)
{
    int handle;


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_WRITE
        );


    if (handle < 0)
    {
        return 0;
    }


    if (
        Bfile_SeekFile(
            handle,
            0
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    if (
        Bfile_WriteFile(
            handle,
            &count,
            sizeof(int)
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    Bfile_CloseFile(handle);


    return 1;
}


/*
    Save a card at index.
*/

static int SaveCard(int index)
{
    int handle;

    int position;


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_WRITE
        );


    if (handle < 0)
    {
        return 0;
    }


    position =
        sizeof(int) +
        (index * sizeof(Card));


    if (
        Bfile_SeekFile(
            handle,
            position
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    if (
        Bfile_WriteFile(
            handle,
            &CurrentCard,
            sizeof(Card)
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    Bfile_CloseFile(handle);


    return 1;
}


/*
    Load a card at index.
*/

static int LoadCard(int index)
{
    int handle;

    int position;


    handle =
        Bfile_OpenFile(
            CardFile,
            _OPENMODE_READ
        );


    if (handle < 0)
    {
        return 0;
    }


    position =
        sizeof(int) +
        (index * sizeof(Card));


    if (
        Bfile_ReadFile(
            handle,
            &CurrentCard,
            sizeof(Card),
            position
        ) < 0
    )
    {
        Bfile_CloseFile(handle);

        return 0;
    }


    Bfile_CloseFile(handle);


    return 1;
}


/* =========================================================
   ADD ATTACK
   ========================================================= */

static int AddAttack(void)
{
    Attack *attack;


    if (
        CurrentCard.attackCount >=
        MAX_ATTACKS
    )
    {
        Message("Maximum 4 attacks");

        return 0;
    }


    attack =
        &CurrentCard.attacks[
            CurrentCard.attackCount
        ];


    attack->name[0] = 0;
    attack->damage[0] = 0;
    attack->energy[0] = 0;
    attack->description[0] = 0;


    if (
        !EditText(
            attack->name,
            NAME_LEN,
            "Attack name:"
        )
    )
    {
        return 0;
    }


    if (
        !EditText(
            attack->damage,
            DAMAGE_LEN,
            "Damage:"
        )
    )
    {
        return 0;
    }


    if (
        !EditText(
            attack->energy,
            ENERGY_LEN,
            "Energy cost:"
        )
    )
    {
        return 0;
    }


    if (
        !EditText(
            attack->description,
            DESC_LEN,
            "Description:"
        )
    )
    {
        return 0;
    }


    CurrentCard.attackCount++;


    return 1;
}


/* =========================================================
   ATTACK MENU
   ========================================================= */

static void AttackMenu(void)
{
    unsigned int key;

    int i;


    while (1)
    {
        ClearScreen();


        locate(1, 1);

        Print(
            (unsigned char *)
            "ATTACKS"
        );


        for (
            i = 0;
            i < CurrentCard.attackCount;
            i++
        )
        {
            locate(
                1,
                i + 3
            );

            Print(
                (unsigned char *)
                CurrentCard
                    .attacks[i]
                    .name
            );
        }


        locate(1, 7);

        Print(
            (unsigned char *)
            "F1 = Add"
        );


        locate(11, 7);

        Print(
            (unsigned char *)
            "EXE = Done"
        );


        GetKey(&key);


        if (
            key == KEY_CTRL_F1
        )
        {
            AddAttack();
        }


        if (
            key == KEY_CTRL_EXE
        )
        {
            return;
        }


        if (
            key == KEY_CTRL_EXIT
        )
        {
            return;
        }
    }
}


/* =========================================================
   ADD CARD
   ========================================================= */

static void AddCard(void)
{
    /*
        Make database if necessary.
    */

    if (!DatabaseExists())
    {
        if (!CreateDatabase())
        {
            Problem();

            return;
        }

        CardCount = 0;
    }


    /*
        Check limit.
    */

    if (CardCount >= MAX_CARDS)
    {
        Message("Card limit reached");

        return;
    }


    ClearCard();


    /*
        CARD NAME
    */

    if (
        !EditText(
            CurrentCard.name,
            NAME_LEN,
            "Card name:"
        )
    )
    {
        return;
    }


    /*
        HP
    */

    if (
        !EditNumber(
            &CurrentCard.hp,
            "HP:"
        )
    )
    {
        return;
    }


    /*
        TYPE 1
    */

    if (
        !EditText(
            CurrentCard.type1,
            TYPE_LEN,
            "Card Type 1:"
        )
    )
    {
        return;
    }


    /*
        TYPE 2
    */

    if (
        !EditText(
            CurrentCard.type2,
            TYPE_LEN,
            "Card Type 2:"
        )
    )
    {
        return;
    }


    /*
        ATTACKS
    */

    AttackMenu();


    /*
        WEAKNESS
    */

    if (
        !EditText(
            CurrentCard.weakness,
            TYPE_LEN,
            "Weakness:"
        )
    )
    {
        return;
    }


    /*
        RESISTANCE
    */

    if (
        !EditText(
            CurrentCard.resistance,
            TYPE_LEN,
            "Resistance:"
        )
    )
    {
        return;
    }


    /*
        RETREAT
    */

    if (
        !EditNumber(
            &CurrentCard.retreat,
            "Retreat cost:"
        )
    )
    {
        return;
    }


    /*
        SAVE CARD
    */

    if (
        !SaveCard(CardCount)
    )
    {
        Problem();

        return;
    }


    CardCount++;


    if (
        !SaveCardCount(CardCount)
    )
    {
        Problem();

        return;
    }


    Message("Card saved!");
}


/* =========================================================
   VIEW CARD
   ========================================================= */

static void ViewCard(int index)
{
    unsigned int key;

    int page;

    int i;


    if (!LoadCard(index))
    {
        Problem();

        return;
    }


    page = 0;


    while (1)
    {
        ClearScreen();


        /*
            PAGE 1
        */

        if (page == 0)
        {
            locate(1, 1);

            Print(
                (unsigned char *)
                CurrentCard.name
            );


            locate(1, 2);

            Print(
                (unsigned char *)
                "HP: "
            );

            PrintInt(
                CurrentCard.hp
            );


            locate(1, 3);

            Print(
                (unsigned char *)
                "Type: "
            );

            Print(
                (unsigned char *)
                CurrentCard.type1
            );


            if (
                CurrentCard.type2[0] != 0
            )
            {
                Print(
                    (unsigned char *)
                    "/"
                );

                Print(
                    (unsigned char *)
                    CurrentCard.type2
                );
            }


            locate(1, 5);

            Print(
                (unsigned char *)
                "Weak: "
            );

            Print(
                (unsigned char *)
                CurrentCard.weakness
            );


            locate(1, 6);

            Print(
                (unsigned char *)
                "Resist: "
            );

            Print(
                (unsigned char *)
                CurrentCard.resistance
            );


            locate(1, 7);

            Print(
                (unsigned char *)
                "Retreat: "
            );

            PrintInt(
                CurrentCard.retreat
            );
        }


        /*
            PAGE 2
        */

        else
        {
            locate(1, 1);

            Print(
                (unsigned char *)
                "ATTACKS"
            );


            if (
                CurrentCard.attackCount == 0
            )
            {
                locate(1, 3);

                Print(
                    (unsigned char *)
                    "No attacks"
                );
            }


            for (
                i = 0;
                i < CurrentCard.attackCount &&
                i < 3;
                i++
            )
            {
                locate(
                    1,
                    (i * 2) + 3
                );

                Print(
                    (unsigned char *)
                    CurrentCard
                        .attacks[i]
                        .name
                );

                Print(
                    (unsigned char *)
                    " "
                );

                Print(
                    (unsigned char *)
                    CurrentCard
                        .attacks[i]
                        .damage
                );
            }
        }


        locate(1, 8);

        Print(
            (unsigned char *)
            "L/R=Page EXIT=Back"
        );


        GetKey(&key);


        if (
            key == KEY_CTRL_EXIT
        )
        {
            return;
        }


        if (
            key == KEY_CTRL_LEFT
        )
        {
            page = 0;
        }


        if (
            key == KEY_CTRL_RIGHT
        )
        {
            page = 1;
        }
    }
}


/* =========================================================
   BROWSE CARDS
   ========================================================= */

static void BrowseCards(void)
{
    int selected;

    unsigned int key;


    if (CardCount == 0)
    {
        Message("No cards");

        return;
    }


    selected = 0;


    while (1)
    {
        ClearScreen();


        if (!LoadCard(selected))
        {
            Problem();

            return;
        }


        locate(1, 1);

        Print(
            (unsigned char *)
            "CARDS"
        );


        locate(1, 2);

        Print(
            (unsigned char *)
            "Card "
        );

        PrintInt(selected + 1);

        Print(
            (unsigned char *)
            "/"
        );

        PrintInt(CardCount);


        locate(1, 4);

        Print(
            (unsigned char *)
            "> "
        );

        Print(
            (unsigned char *)
            CurrentCard.name
        );


        locate(1, 5);

        Print(
            (unsigned char *)
            "HP: "
        );

        PrintInt(
            CurrentCard.hp
        );


        locate(1, 6);

        Print(
            (unsigned char *)
            "Type: "
        );

        Print(
            (unsigned char *)
            CurrentCard.type1
        );


        locate(1, 8);

        Print(
            (unsigned char *)
            "UP/DN EXE=View"
        );


        GetKey(&key);


        if (
            key == KEY_CTRL_UP
        )
        {
            if (selected > 0)
            {
                selected--;
            }
        }


        if (
            key == KEY_CTRL_DOWN
        )
        {
            if (
                selected <
                CardCount - 1
            )
            {
                selected++;
            }
        }


        if (
            key == KEY_CTRL_EXE
        )
        {
            ViewCard(selected);
        }


        if (
            key == KEY_CTRL_EXIT
        )
        {
            return;
        }
    }
}


/* =========================================================
   MAIN MENU
   ========================================================= */

static void MainMenu(void)
{
    int selected;

    unsigned int key;


    selected = 0;


    while (1)
    {
        ClearScreen();


        locate(1, 1);

        Print(
            (unsigned char *)
            "POKEMON CARDS"
        );


        locate(1, 2);

        Print(
            (unsigned char *)
            "Cards: "
        );

        PrintInt(CardCount);


        /*
            VIEW
        */

        locate(1, 4);

        if (selected == 0)
        {
            Print(
                (unsigned char *)
                "> View Cards"
            );
        }
        else
        {
            Print(
                (unsigned char *)
                "  View Cards"
            );
        }


        /*
            ADD
        */

        locate(1, 5);

        if (selected == 1)
        {
            Print(
                (unsigned char *)
                "> Add Card"
            );
        }
        else
        {
            Print(
                (unsigned char *)
                "  Add Card"
            );
        }


        locate(1, 8);

        Print(
            (unsigned char *)
            "UP/DN EXE=Select"
        );


        GetKey(&key);


        if (
            key == KEY_CTRL_UP
        )
        {
            if (selected > 0)
            {
                selected--;
            }
        }


        if (
            key == KEY_CTRL_DOWN
        )
        {
            if (selected < 1)
            {
                selected++;
            }
        }


        if (
            key == KEY_CTRL_EXE
        )
        {
            if (selected == 0)
            {
                BrowseCards();
            }
            else
            {
                AddCard();
            }
        }


        if (
            key == KEY_CTRL_EXIT
        )
        {
            return;
        }
    }
}


/* =========================================================
   MAIN ENTRY
   ========================================================= */

int AddIn_main(
    int isAppli,
    unsigned short OptionNum
)
{
    /*
        Missing CARDS.DAT is NOT an error.

        The file is created automatically
        when the first card is saved.
    */

    if (
        DatabaseExists()
    )
    {
        CardCount =
            LoadCardCount();
    }
    else
    {
        CardCount = 0;
    }


    MainMenu();


    return 1;
}


/* =========================================================
   REQUIRED CASIO SDK SECTIONS
   ========================================================= */

#pragma section _BR_Size

unsigned long BR_Size;

#pragma section


#pragma section _TOP

int InitializeSystem(
    int isAppli,
    unsigned short OptionNum
)
{
    return INIT_ADDIN_APPLICATION(
        isAppli,
        OptionNum
    );
}

#pragma section