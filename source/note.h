#ifndef NOTE_
#define NOTE_

typedef enum notetype {
    PURPLE = 4,
    BLUE = 1,
    GREEN = 2,
    RED = 5
};

typedef struct Note {
    ML_Sprite note;
    enum notetype type
};
#endif