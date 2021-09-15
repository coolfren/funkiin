#ifndef NOTE_
#define NOTE_

typedef enum notetype {
    PURPLE = 3,
    BLUE = 1,
    GREEN = 2,
    RED = 4
};

typedef struct DaNote {
    ML_Sprite note;
    enum notetype type;
    bool musthit;
    u16 strumtime;
    u16 sustime; // when the impostor is sus!
    bool held;
} Note;

typedef struct cammy {
    u16 x;
    u16 y;
    f32 zoom;
} cam;

typedef struct cond {
    u8 bpm;
	f32 crochet; // beats in milliseconds
	f32 stepCrochet; // steps in milliseconds
	f32 songPosition;
	f32 offset;
} con;
#endif