#ifndef VALUE_H
#define VALUE_H

typedef enum {
  VALUE_INVALID,
  VALUE_TEXT,
  VALUE_NUMBER
} ValueType;

typedef struct {
  ValueType type;
  union {
    struct {
      uint8_t length;
      char* chars;
    } text;
    int16_t number;
  } as;
} Value;

#define NUMBER_VAL(value) ((Value){ VALUE_NUMBER, { .number = value } })
#define TEXT_VAL(value) ((Value){ VALUE_TEXT, { .text = { strlen(value), value } } })

#define AS_TEXT(value)    ((value).as.text.chars)
#define AS_NUMBER(value)  ((value).as.number)

#define IS_NUMBER(value)  ((value).type == VALUE_NUMBER)
#define IS_TEXT(value)  ((value).type == VALUE_TEXT)

#endif
