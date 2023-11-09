#include <stdlib.h>
#include <stdio.h>

/*
// C2V related macros
#define INT_C2V (3)
#define FRAC_C2V (4)
#define RESOLUTION_C2V (1.0 / (1 << FRAC_C2V))
#define HALF_RESOLUTION_C2V (RESOLUTION_C2V / 2)
#define MAX_C2V ((1 << INT_C2V) - RESOLUTION_C2V)

// channel LLR related macros
#define INT_CH_LLR (3)
#define FRAC_CH_LLR (4)
#define RESOLUTION_CH_LLR (1.0 / (1 << FRAC_CH_LLR))
#define HALF_RESOLUTION_CH_LLR (RESOLUTION_CH_LLR / 2)
#define MAX_CH_LLR ((1 << INT_CH_LLR) - RESOLUTION_CH_LLR)

// V2C related macros
#define INT_V2C (5)
#define FRAC_V2C (4)
#define RESOLUTION_V2C (1.0 / (1 << FRAC_V2C))
#define HALF_RESOLUTION_V2C (RESOLUTION_V2C / 2)
#define MAX_V2C ((1 << INT_V2C) - RESOLUTION_V2C)

// a posteriori probability (APP, decision LLR) related macros
#define INT_APP (5)
#define FRAC_APP (4)
#define RESOLUTION_APP (1.0 / (1 << FRAC_APP))
#define HALF_RESOLUTION_APP (RESOLUTION_APP / 2)
#define MAX_APP ((1 << INT_APP) - RESOLUTION_APP)
*/

/*
// C2V related macros
#define RESOLUTION_C2V (0.125)
#define HALF_RESOLUTION_C2V (RESOLUTION_C2V / 2)
#define MAX_C2V (27.9)

// channel LLR related macros
#define RESOLUTION_CH_LLR (RESOLUTION_C2V)
#define HALF_RESOLUTION_CH_LLR (RESOLUTION_CH_LLR / 2)
#define MAX_CH_LLR (MAX_C2V)

// V2C related macros
#define RESOLUTION_V2C (RESOLUTION_C2V)
#define HALF_RESOLUTION_V2C (RESOLUTION_V2C / 2)
#define MAX_V2C (MAX_C2V)

// a posteriori probability (APP, decision LLR) related macros
#define RESOLUTION_APP (RESOLUTION_C2V)
#define HALF_RESOLUTION_APP (RESOLUTION_APP / 2)
#define MAX_APP (MAX_C2V)
*/

#define DELTA (0.001)







#define RESOLUTION (0.40)
#define HALF_RESOLUTION (RESOLUTION / 2)

// channel LLR related macros
#define RESOLUTION_CH_LLR (RESOLUTION)
#define HALF_RESOLUTION_CH_LLR (RESOLUTION_CH_LLR / 2)
#define MAX_CH_LLR (63 * RESOLUTION_CH_LLR)

// C2V related macros
#define RESOLUTION_C2V (RESOLUTION)
#define HALF_RESOLUTION_C2V (RESOLUTION_C2V / 2)
#define MAX_C2V (63 * RESOLUTION_C2V)

// V2C related macros
#define RESOLUTION_V2C (RESOLUTION)
#define HALF_RESOLUTION_V2C (RESOLUTION_V2C / 2)
#define MAX_V2C (63 * RESOLUTION_V2C)

// a posteriori probability (APP, decision LLR) related macros
#define RESOLUTION_APP (RESOLUTION_V2C)
#define HALF_RESOLUTION_APP (RESOLUTION_APP / 2)
#define MAX_APP (MAX_V2C)



















//a posteriori probability for extend parity bit (APP, decision LLR) related macros
#define RESOLUTION_EXTEN_APP (0.20)
#define HALF_RESOLUTION_EXTEN_APP (RESOLUTION_EXTEN_APP / 2)
#define MAX_EXTEN_APP (25.4)

// channel LLR re-quan related macros
#define RESOLUTION_CH_LLR_REQUAN (RESOLUTION)
#define HALF_RESOLUTION_CH_LLR_REQUAN (RESOLUTION_CH_LLR_REQUAN / 2)
#define MAX_CH_LLR_REQUAN (15 * RESOLUTION_CH_LLR_REQUAN)

double AdaptiveChReQuanUp(double input)
{
    int sign = 1;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_CH_LLR_REQUAN - HALF_RESOLUTION_CH_LLR_REQUAN)) {
        input = MAX_CH_LLR_REQUAN;
    } else {
        input = ((int)((input + HALF_RESOLUTION_CH_LLR_REQUAN) / RESOLUTION_CH_LLR_REQUAN)) * RESOLUTION_CH_LLR_REQUAN;
    }

    return sign * input;
}

double AdaptiveChReQuanDown(double input)
{
    int sign = 1;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input > (MAX_CH_LLR_REQUAN - HALF_RESOLUTION_CH_LLR_REQUAN)) {
        input = MAX_CH_LLR_REQUAN;
    } else {
        input = ((int)((input + HALF_RESOLUTION_CH_LLR_REQUAN * 0.9) / RESOLUTION_CH_LLR_REQUAN)) * RESOLUTION_CH_LLR_REQUAN;
    }

    return sign * input;
}

double AdaptiveChReQuanDirectDrop(double input)
{
    int sign = 1;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= MAX_CH_LLR_REQUAN) {
        input = MAX_CH_LLR_REQUAN;
    } else {
        input = ((int)((input + DELTA) / RESOLUTION_CH_LLR_REQUAN)) * RESOLUTION_CH_LLR_REQUAN;
    }

    return sign * input;
}

double QuanC2V(double input)
{
    int sign = 1;
    
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_C2V - HALF_RESOLUTION_C2V)) {
        input = MAX_C2V;
    } else {
        input = ((int)((input + HALF_RESOLUTION_C2V) / RESOLUTION_C2V)) * RESOLUTION_C2V;
    }

    return sign * input;
}

double QuanC2V7bTo5bTail0(double input)                // 7bit mag to 5bit mag, 5bit mag has double resolution
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3e;
    } else {
        inputBinary = inputBinary & 0x3e;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo5bTail1(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3f;
    } else {
        inputBinary = (inputBinary & 0x3f) | 0x1;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo4bTail00(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3c;
    } else {
        inputBinary = inputBinary & 0x3c;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo4bTail01(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3d;
    } else {
        inputBinary = (inputBinary & 0x3c) | 0x1;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo4bTail10(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3e;
    } else {
        inputBinary = (inputBinary & 0x3c) | 0x2;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo4bTail11(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x1 << 6)) != 0) {
        inputBinary = 0x3f;
    } else {
        inputBinary = (inputBinary & 0x3c) | 0x3;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}


double QuanC2V7bToLow4bTail0(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x3 << 5)) != 0) {
        inputBinary = 0x1e;
    } else {
        inputBinary = inputBinary & 0x1e;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bToLow4bTail1(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x3 << 5)) != 0) {
        inputBinary = 0x1f;
    } else {
        inputBinary = (inputBinary & 0x1e) | 0x1;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bToLowest4b(double input)
{
    int inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary & (0x7 << 4)) != 0) {
        inputBinary = 0xf;
    } else {
        inputBinary = inputBinary & 0xf;
    }

    input = inputBinary * RESOLUTION_C2V;

    return input;
}



double NonC2vUniQuan6bTo4bA(double input)
{
    int inputBinary;
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 12) && (inputBinary <= 19)) {
        if (inputBinary <= 15) {
            inputBinary = 14;
        } else {
            inputBinary = 18;
        }
    } else if ((inputBinary >= 20) && (inputBinary <= 25)) {
        inputBinary = 23;
    } else if ((inputBinary >= 26) && (inputBinary <= 33)) {
        inputBinary = 30;
    } else if ((inputBinary >= 34) && (inputBinary <= 45)) {
        inputBinary = 40;
    } else if ((inputBinary >= 46) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}

double NonC2vUniQuan6bTo4bB(double input)
{
    int inputBinary;
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 28)) {
        if (inputBinary <= 22) {
            inputBinary = 20;
        } else {
            inputBinary = 26;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 38)) {
        inputBinary = 34;
    } else if ((inputBinary >= 39) && (inputBinary <= 52)) {
        inputBinary = 46;
    } else if ((inputBinary >= 53) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}



double NonC2vUniQuan6bTo4bC(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 24)) {
        if (inputBinary <= 20) {
            inputBinary = 19;
        } else {
            inputBinary = 23;
        }
    } else if ((inputBinary >= 25) && (inputBinary <= 30)) {
        inputBinary = 28;
    } else if ((inputBinary >= 31) && (inputBinary <= 37)) {
        inputBinary = 34;
    } else if ((inputBinary >= 38) && (inputBinary <= 63)) {
        inputBinary = 51;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}

double NonC2vUniQuan6bTo4bD(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 7) && (inputBinary <= 12)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 13) && (inputBinary <= 18)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 19) && (inputBinary <= 23)) {
        inputBinary = 21;
    } else if ((inputBinary >= 24) && (inputBinary <= 31)) {
        inputBinary = 28;
    } else if ((inputBinary >= 32) && (inputBinary <= 44)) {
        inputBinary = 38;
    } else if ((inputBinary >= 45) && (inputBinary <= 63)) {
        inputBinary = 54;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}

double NonC2vUniQuan6bTo4bE(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 26)) {
        if (inputBinary <= 21) {
            inputBinary = 19;
        } else {
            inputBinary = 24;
        }
    } else if ((inputBinary >= 27) && (inputBinary <= 33)) {
        inputBinary = 30;
    } else if ((inputBinary >= 34) && (inputBinary <= 45)) {
        inputBinary = 35;
    } else if ((inputBinary >= 46) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}


double NonC2vUniQuan6bTo4bF(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 5) && (inputBinary <= 11)) {
        if ((inputBinary & 0x1) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 12) && (inputBinary <= 17)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 18) && (inputBinary <= 25)) {
        if (inputBinary <= 21) {
            inputBinary = 20;
        } else {
            inputBinary = 24;
        }
    } else if ((inputBinary >= 26) && (inputBinary <= 34)) {
        inputBinary = 30;
    } else if ((inputBinary >= 35) && (inputBinary <= 48)) {
        inputBinary = 42;
    } else if ((inputBinary >= 49) && (inputBinary <= 63)) {
        inputBinary = 56;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}


double NonC2vUniQuan6bTo4bG(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 10) && (inputBinary <= 13)) {
        if ((inputBinary & 0x1) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 14) && (inputBinary <= 17)) {
        inputBinary = 16;
    } else if ((inputBinary >= 18) && (inputBinary <= 27)) {
        inputBinary = 23;
    } else if ((inputBinary >= 28) && (inputBinary <= 43)) {
        inputBinary = 36;
    } else if ((inputBinary >= 44) && (inputBinary <= 63)) {
        inputBinary = 51;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}

double NonC2vUniQuan6bTo4bH(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 9) && (inputBinary <= 12)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 13) && (inputBinary <= 22)) {
        if (inputBinary <= 17) {
            inputBinary = 15;
        } else {
            inputBinary = 20;
        }
    } else if ((inputBinary >= 23) && (inputBinary <= 31)) {
        inputBinary = 27;
    } else if ((inputBinary >= 32) && (inputBinary <= 46)) {
        inputBinary = 39;
    } else if ((inputBinary >= 47) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = inputBinary * RESOLUTION_C2V;
    return input;
}

double QuanC2V7bTo6bTo5bTail0(double input)
{
    int inputBinary;

    input = QuanC2V(input);    // 7b to 6b, saturation
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);
    inputBinary = inputBinary & 0x3e;

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double QuanC2V7bTo6bTo5bTail1(double input)
{
    int inputBinary;

    input = QuanC2V(input);    // 7b to 6b, saturation
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);
    inputBinary = (inputBinary & 0x3f) | 0x1;

    input = inputBinary * RESOLUTION_C2V;

    return input;
}

double C2VNonUniQuan6bTo5b(double input)
{
    int inputBinary;

    inputBinary = (int)((input + DELTA) / RESOLUTION_C2V);

    if ((inputBinary >= 16) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 41)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 42) && (inputBinary <= 49)) {
        if (inputBinary <= 45) {
            inputBinary = 44;
        } else {
            inputBinary = 48;
        }
    } else if ((inputBinary >= 50) && (inputBinary <= 59)) {
        if (inputBinary <= 54) {
            inputBinary = 52;
        } else {
            inputBinary = 57;
        }
    } else if ((inputBinary >= 60) && (inputBinary <= 63)) {
        inputBinary = 63;
    }

    return inputBinary * RESOLUTION_C2V;
}

double NonUniQuan7bTo6bA(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 16) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 41)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 42) && (inputBinary <= 49)) {
        if (inputBinary <= 45) {
            inputBinary = 44;
        } else {
            inputBinary = 48;
        }
    } else if ((inputBinary >= 50) && (inputBinary <= 59)) {
        if (inputBinary <= 54) {
            inputBinary = 52;
        } else {
            inputBinary = 57;
        }
    } else if ((inputBinary >= 60) && (inputBinary <= 63)) {
        inputBinary = 62;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}


double NonUniQuan7bTo6bB(double input)   // a little big
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 33)) {
        inputBinary |= 0x1;
    } else if ((inputBinary >= 34) && (inputBinary <= 42)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 43) && (inputBinary <= 46)) {
        inputBinary = 45;
    } else if ((inputBinary >= 47) && (inputBinary <= 51)) {
        inputBinary = 49;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bC(double input)   // a little small
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 33)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 34) && (inputBinary <= 42)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 43) && (inputBinary <= 46)) {
        inputBinary = 44;
    } else if ((inputBinary >= 47) && (inputBinary <= 51)) {
        inputBinary = 49;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bD(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 14) && (inputBinary <= 31)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 32) && (inputBinary <= 49)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 50) && (inputBinary <= 57)) {
        if (inputBinary <= 53) {
            inputBinary = 52;
        } else {
            inputBinary = 56;
        }
    } else if ((inputBinary >= 58) && (inputBinary <= 63)) {
        inputBinary = 61;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}


double NonUniQuan7bTo6bE(double input)   // much big
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 33)) {
        inputBinary |= 0x1;
    } else if ((inputBinary >= 34) && (inputBinary <= 42)) {
        if ((inputBinary % 3) == 1) {
            inputBinary += 2;
        } else if ((inputBinary % 3) == 2) {
            inputBinary++;
        }
    } else if ((inputBinary >= 43) && (inputBinary <= 46)) {
        inputBinary = 46;
    } else if ((inputBinary >= 47) && (inputBinary <= 51)) {
        inputBinary = 51;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 63;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bF(double input)   // much small
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 33)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 34) && (inputBinary <= 42)) {
        if ((inputBinary % 3) == 2) {
            inputBinary--;
        } else if ((inputBinary % 3) == 0) {
            inputBinary -= 2;
        }
    } else if ((inputBinary >= 43) && (inputBinary <= 46)) {
        inputBinary = 43;
    } else if ((inputBinary >= 47) && (inputBinary <= 51)) {
        inputBinary = 47;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 52;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bG(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 20) && (inputBinary <= 35)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 36) && (inputBinary <= 41)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 42) && (inputBinary <= 50)) {
        inputBinary = 46;
    } else if ((inputBinary >= 51) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bG1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 20) && (inputBinary <= 35)) {
        inputBinary |= 0x1;
    } else if ((inputBinary >= 36) && (inputBinary <= 41)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 42) && (inputBinary <= 50)) {
        inputBinary = 46;
    } else if ((inputBinary >= 51) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bH(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 23) && (inputBinary <= 30)) {
        if ((inputBinary % 2) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 36)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 40)) {
        inputBinary = 39;
    } else if ((inputBinary >= 41) && (inputBinary <= 48)) {
        inputBinary = 45;
    } else if ((inputBinary >= 49) && (inputBinary <= 63)) {
        inputBinary = 56;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bH1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 23) && (inputBinary <= 30)) {
        if ((inputBinary % 2) == 1) {
            inputBinary++;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 36)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 40)) {
        inputBinary = 39;
    } else if ((inputBinary >= 41) && (inputBinary <= 48)) {
        inputBinary = 45;
    } else if ((inputBinary >= 49) && (inputBinary <= 63)) {
        inputBinary = 56;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        inputBinary |= 0x1;
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI001(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 24;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 26;
        } else {                         // 28 29
            inputBinary = 29;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI010(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 24;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 27;
        } else {                         // 28 29
            inputBinary = 28;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI011(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 24;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 27;
        } else {                         // 28 29
            inputBinary = 29;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI100(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 25;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 26;
        } else {                         // 28 29
            inputBinary = 28;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI101(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 25;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 26;
        } else {                         // 28 29
            inputBinary = 29;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bI110(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 24) && (inputBinary <= 29)) {
        if (inputBinary <= 25) {         // 24 25
            inputBinary = 25;
        } else if (inputBinary <= 27) {  // 26 27
            inputBinary = 27;
        } else {                         // 28 29
            inputBinary = 28;
        }
    } else if ((inputBinary >= 30) && (inputBinary <= 37)) {
        if (inputBinary <= 33) {
            inputBinary = 32;
        } else {
            inputBinary = 36;
        }
    } else if ((inputBinary >= 38) && (inputBinary <= 43)) {
        inputBinary = 41;
    } else if ((inputBinary >= 44) && (inputBinary <= 51)) {
        inputBinary = 48;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bJ(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 28)) {
        if ((inputBinary % 2) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 36)) {
        if (inputBinary <= 32) {
            inputBinary = 31;
        } else {
            inputBinary = 35;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 42)) {
        inputBinary = 40;
    } else if ((inputBinary >= 43) && (inputBinary <= 51)) {
        inputBinary = 47;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bJ01(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 28)) {
        if (inputBinary <= 26) {  // 25 26
            inputBinary = 25;
        } else {                  // 27 28
            inputBinary = 28;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 36)) {
        if (inputBinary <= 32) {
            inputBinary = 31;
        } else {
            inputBinary = 35;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 42)) {
        inputBinary = 40;
    } else if ((inputBinary >= 43) && (inputBinary <= 51)) {
        inputBinary = 47;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bJ10(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 28)) {
        if (inputBinary <= 26) {  // 25 26
            inputBinary = 26;
        } else {                  // 27 28
            inputBinary = 27;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 36)) {
        if (inputBinary <= 32) {
            inputBinary = 31;
        } else {
            inputBinary = 35;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 42)) {
        inputBinary = 40;
    } else if ((inputBinary >= 43) && (inputBinary <= 51)) {
        inputBinary = 47;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bJ1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 28)) {
        if ((inputBinary % 2) == 1) {
            inputBinary++;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 36)) {
        if (inputBinary <= 32) {
            inputBinary = 31;
        } else {
            inputBinary = 35;
        }
    } else if ((inputBinary >= 37) && (inputBinary <= 42)) {
        inputBinary = 40;
    } else if ((inputBinary >= 43) && (inputBinary <= 51)) {
        inputBinary = 47;
    } else if ((inputBinary >= 52) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if ((inputBinary % 2) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK001(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 25;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 27;
        } else {                         // 29 30
            inputBinary = 30;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK010(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 25;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 28;
        } else {                         // 29 30
            inputBinary = 29;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK011(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 25;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 28;
        } else {                         // 29 30
            inputBinary = 30;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK100(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 26;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 27;
        } else {                         // 29 30
            inputBinary = 29;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK101(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 26;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 27;
        } else {                         // 29 30
            inputBinary = 30;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK110(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if (inputBinary <= 26) {         // 25 26
            inputBinary = 26;
        } else if (inputBinary <= 28) {  // 27 28
            inputBinary = 28;
        } else {                         // 29 30
            inputBinary = 29;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo6bK1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 25) && (inputBinary <= 30)) {
        if ((inputBinary % 2) == 1) {
            inputBinary++;
        }
    } else if ((inputBinary >= 31) && (inputBinary <= 33)) {
        inputBinary = 32;
    } else if ((inputBinary >= 34) && (inputBinary <= 39)) {
        inputBinary = 37;
    } else if ((inputBinary >= 40) && (inputBinary <= 49)) {
        inputBinary = 45;
    } else if ((inputBinary >= 50) && (inputBinary <= 63)) {
        inputBinary = 57;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bA(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 16) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 41)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 42) && (inputBinary <= 49)) {
        if (inputBinary <= 45) {
            inputBinary = 44;
        } else {
            inputBinary = 48;
        }
    } else if ((inputBinary >= 50) && (inputBinary <= 54)) {
        inputBinary = 52;
    } else if ((inputBinary >= 55) && (inputBinary <= 60)) {
        inputBinary = 58;
    } else if ((inputBinary >= 61) && (inputBinary <= 127)) {
        inputBinary = 94;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bB(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 31)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 32) && (inputBinary <= 40)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 41) && (inputBinary <= 48)) {
        if (inputBinary <= 44) {
            inputBinary = 43;
        } else {
            inputBinary = 47;
        }
    } else if ((inputBinary >= 49) && (inputBinary <= 54)) {
        inputBinary = 52;
    } else if ((inputBinary >= 55) && (inputBinary <= 60)) {
        inputBinary = 58;
    } else if ((inputBinary >= 61) && (inputBinary <= 127)) {
        inputBinary = 94;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bC(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 31)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 32) && (inputBinary <= 40)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 41) && (inputBinary <= 48)) {
        if (inputBinary <= 44) {
            inputBinary = 43;
        } else {
            inputBinary = 47;
        }
    } else if ((inputBinary >= 49) && (inputBinary <= 54)) {
        inputBinary = 54;
    } else if ((inputBinary >= 55) && (inputBinary <= 60)) {
        inputBinary = 60;
    } else if ((inputBinary >= 61) && (inputBinary <= 127)) {
        inputBinary = 94;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bD(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 14) && (inputBinary <= 31)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 32) && (inputBinary <= 49)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 50) && (inputBinary <= 57)) {
        if (inputBinary <= 53) {
            inputBinary = 52;
        } else {
            inputBinary = 56;
        }
    } else if ((inputBinary >= 58) && (inputBinary <= 127)) {
        inputBinary = 93;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bE(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 14) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 44)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 45) && (inputBinary <= 56)) {
        if (inputBinary <= 48) {
            inputBinary = 47;
        } else if (inputBinary <= 52) {
            inputBinary = 51;
        } else {
            inputBinary = 55;
        }
    } else if ((inputBinary >= 57) && (inputBinary <= 61)) {
        inputBinary = 59;
    } else if ((inputBinary >= 62) && (inputBinary <= 127)) {
        inputBinary = 95;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bF(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 14) && (inputBinary <= 27)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 28) && (inputBinary <= 39)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 40) && (inputBinary <= 51)) {
        if (inputBinary <= 43) {
            inputBinary = 42;
        } else if (inputBinary <= 47) {
            inputBinary = 46;
        } else {
            inputBinary = 50;
        }
    } else if ((inputBinary >= 52) && (inputBinary <= 61)) {
        if (inputBinary <= 56) {
            inputBinary = 54;
        } else {
            inputBinary = 59;
        }
    } else if ((inputBinary >= 62) && (inputBinary <= 71)) {
        inputBinary = 67;
    } else if ((inputBinary >= 72) && (inputBinary <= 127)) {
        inputBinary = 100;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bG(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 19) && (inputBinary <= 32)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 33) && (inputBinary <= 44)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 45) && (inputBinary <= 74)) {
        inputBinary = 60;
    } else if ((inputBinary >= 75) && (inputBinary <= 127)) {
        inputBinary = 101;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bH(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 17) && (inputBinary <= 28)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 44)) {
        if ((inputBinary % 4) == 1) {
            inputBinary += 2;
        } else if ((inputBinary % 4) == 2) {
            inputBinary++;
        } else if ((inputBinary % 4) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 45) && (inputBinary <= 68)) {
        if (inputBinary <= 52) {
            inputBinary = 49;
        } else if (inputBinary <= 60) {
            inputBinary = 57;
        } else {
            inputBinary = 65;
        }
    } else if ((inputBinary >= 69) && (inputBinary <= 84)) {
        inputBinary = 77;
    } else if ((inputBinary >= 85) && (inputBinary <= 127)) {
        inputBinary = 106;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bI(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 18) && (inputBinary <= 29)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 30) && (inputBinary <= 53)) {
            inputBinary = inputBinary + (3 - (inputBinary % 6));
    } else if ((inputBinary >= 54) && (inputBinary <= 77)) {
        if (inputBinary <= 65) {
            inputBinary = 60;
        } else {
            inputBinary = 72;
        }
    } else if ((inputBinary >= 78) && (inputBinary <= 97)) {
        inputBinary = 88;
    } else if ((inputBinary >= 98) && (inputBinary <= 127)) {
        inputBinary = 113;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo6bJ(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 17) && (inputBinary <= 26)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 27) && (inputBinary <= 42)) {
            inputBinary = inputBinary + (2 - ((inputBinary - 27) % 4));
    } else if ((inputBinary >= 43) && (inputBinary <= 66)) {
        if (inputBinary <= 50) {
            inputBinary = 47;
        } else if (inputBinary <= 58) {
            inputBinary = 55;
        } else {
            inputBinary = 63;
        }
    } else if ((inputBinary >= 67) && (inputBinary <= 80)) {
        inputBinary = 74;
    } else if ((inputBinary >= 81) && (inputBinary <= 100)) {
        inputBinary = 91;
    } else if ((inputBinary >= 101) && (inputBinary <= 127)) {
        inputBinary = 114;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}
















double NonUniQuan7bTo5bA(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 12) && (inputBinary <= 19)) {
        if (inputBinary <= 15) {
            inputBinary = 14;
        } else {
            inputBinary = 18;
        }
    } else if ((inputBinary >= 20) && (inputBinary <= 25)) {
        inputBinary = 23;
    } else if ((inputBinary >= 26) && (inputBinary <= 33)) {
        inputBinary = 30;
    } else if ((inputBinary >= 34) && (inputBinary <= 45)) {
        inputBinary = 40;
    } else if ((inputBinary >= 46) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo5bB(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 28)) {
        if (inputBinary <= 22) {
            inputBinary = 20;
        } else {
            inputBinary = 26;
        }
    } else if ((inputBinary >= 29) && (inputBinary <= 38)) {
        inputBinary = 34;
    } else if ((inputBinary >= 39) && (inputBinary <= 52)) {
        inputBinary = 46;
    } else if ((inputBinary >= 53) && (inputBinary <= 63)) {
        inputBinary = 58;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}



double NonUniQuan7bTo5bC(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 24)) {
        if (inputBinary <= 20) {
            inputBinary = 19;
        } else {
            inputBinary = 23;
        }
    } else if ((inputBinary >= 25) && (inputBinary <= 30)) {
        inputBinary = 28;
    } else if ((inputBinary >= 31) && (inputBinary <= 37)) {
        inputBinary = 34;
    } else if ((inputBinary >= 38) && (inputBinary <= 63)) {
        inputBinary = 51;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo5bD(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 7) && (inputBinary <= 12)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 13) && (inputBinary <= 18)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 19) && (inputBinary <= 23)) {
        inputBinary = 21;
    } else if ((inputBinary >= 24) && (inputBinary <= 31)) {
        inputBinary = 28;
    } else if ((inputBinary >= 32) && (inputBinary <= 44)) {
        inputBinary = 38;
    } else if ((inputBinary >= 45) && (inputBinary <= 63)) {
        inputBinary = 54;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo5bE(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 7) && (inputBinary <= 10)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 11) && (inputBinary <= 16)) {
        if ((inputBinary % 3) == 2) {
            inputBinary++;
        } else if ((inputBinary % 3) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 17) && (inputBinary <= 26)) {
        if (inputBinary <= 21) {
            inputBinary = 19;
        } else {
            inputBinary = 24;
        }
    } else if ((inputBinary >= 27) && (inputBinary <= 33)) {
        inputBinary = 30;
    } else if ((inputBinary >= 34) && (inputBinary <= 45)) {
        inputBinary = 35;
    } else if ((inputBinary >= 46) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}


double NonUniQuan7bTo5bF(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 11)) {
        if ((inputBinary & 0x1) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 12) && (inputBinary <= 17)) {
        if ((inputBinary % 3) == 0) {
            inputBinary++;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 18) && (inputBinary <= 25)) {
        if (inputBinary <= 21) {
            inputBinary = 20;
        } else {
            inputBinary = 24;
        }
    } else if ((inputBinary >= 26) && (inputBinary <= 34)) {
        inputBinary = 30;
    } else if ((inputBinary >= 35) && (inputBinary <= 48)) {
        inputBinary = 42;
    } else if ((inputBinary >= 49) && (inputBinary <= 63)) {
        inputBinary = 56;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}


double NonUniQuan7bTo5bG(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 10) && (inputBinary <= 13)) {
        if ((inputBinary & 0x1) == 1) {
            inputBinary--;
        }
    } else if ((inputBinary >= 14) && (inputBinary <= 17)) {
        inputBinary = 16;
    } else if ((inputBinary >= 18) && (inputBinary <= 27)) {
        inputBinary = 23;
    } else if ((inputBinary >= 28) && (inputBinary <= 43)) {
        inputBinary = 36;
    } else if ((inputBinary >= 44) && (inputBinary <= 63)) {
        inputBinary = 54;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan7bTo5bH(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 9) && (inputBinary <= 12)) {
        if ((inputBinary & 0x1) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 13) && (inputBinary <= 22)) {
        if (inputBinary <= 17) {
            inputBinary = 15;
        } else {
            inputBinary = 20;
        }
    } else if ((inputBinary >= 23) && (inputBinary <= 31)) {
        inputBinary = 27;
    } else if ((inputBinary >= 32) && (inputBinary <= 46)) {
        inputBinary = 39;
    } else if ((inputBinary >= 47) && (inputBinary <= 63)) {
        inputBinary = 55;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo7b(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 40) && (inputBinary <= 63)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 64) && (inputBinary <= 87)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 88) && (inputBinary <= 95)) {
        if (inputBinary <= 91) {
            inputBinary = 90;
        } else {
            inputBinary = 94;
        }
    } else if ((inputBinary >= 96) && (inputBinary <= 127)) {
        if (inputBinary <= 111) {
            inputBinary = 104;
        } else {
            inputBinary = 127;
        }
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo7bMedium(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 40) && (inputBinary <= 63)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 64) && (inputBinary <= 87)) {
        if ((inputBinary % 3) == 1) {
            inputBinary++;
        } else if ((inputBinary % 3) == 0) {
            inputBinary--;
        }
    } else if ((inputBinary >= 88) && (inputBinary <= 95)) {
        if (inputBinary <= 91) {
            inputBinary = 90;
        } else {
            inputBinary = 94;
        }
    } else if ((inputBinary >= 96) && (inputBinary <= 127)) {
        if (inputBinary <= 111) {
            inputBinary = 111;
        } else {
            inputBinary = 127;
        }
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo7bBig(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 40) && (inputBinary <= 63)) {
        inputBinary |= 0x1;
    } else if ((inputBinary >= 64) && (inputBinary <= 87)) {
        if ((inputBinary % 3) == 1) {
            inputBinary += 2;
        } else if ((inputBinary % 3) == 2) {
            inputBinary++;
        }
    } else if ((inputBinary >= 88) && (inputBinary <= 95)) {
        if (inputBinary <= 91) {
            inputBinary = 91;
        } else {
            inputBinary = 95;
        }
    } else if ((inputBinary >= 96) && (inputBinary <= 127)) {
        if (inputBinary <= 111) {
            inputBinary = 111;
        } else {
            inputBinary = 127;
        }
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan8bTo7bSmall(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 40) && (inputBinary <= 63)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 64) && (inputBinary <= 87)) {
        if ((inputBinary % 3) == 0) {
            inputBinary -= 2;
        } else if ((inputBinary % 3) == 2) {
            inputBinary--;
        }
    } else if ((inputBinary >= 88) && (inputBinary <= 95)) {
        if (inputBinary <= 91) {
            inputBinary = 88;
        } else {
            inputBinary = 92;
        }
    } else if ((inputBinary >= 96) && (inputBinary <= 127)) {
        if (inputBinary <= 111) {
            inputBinary = 96;
        } else {
            inputBinary = 112;
        }
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA813(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 8;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA814(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 8;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA815(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 8;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA911(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 11;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA912(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA914(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA915(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA1011(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 11;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA1012(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA1013(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA1014(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA1015(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 7)) {
        inputBinary &= 0xFFFFFFFE;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bAdown(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 8;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bAup(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA47(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA47914(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA47915(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA471014(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA471015(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA57914(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA571015(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bA4710(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bB(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 8)) {
        inputBinary = 7;
    } else if ((inputBinary >= 9) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bBdown0(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 8)) {
        inputBinary = 6;
    } else if ((inputBinary >= 9) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bBdown1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 8)) {
        inputBinary = 6;
    } else if ((inputBinary >= 9) && (inputBinary <= 12)) {
        inputBinary = 10;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bBup0(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 8)) {
        inputBinary = 7;
    } else if ((inputBinary >= 9) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bB8(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 8)) {
        inputBinary = 8;
    } else if ((inputBinary >= 9) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bC(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD813(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 8;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD814(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 8;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD815(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 8;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD912(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD913(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD914(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD915(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1012(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1013(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1015(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1112(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 11;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1113(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 11;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1114(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 11;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bD1115(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 11;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bDdown0(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 8;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bDup0(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bDup1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 11;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bE(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 5;
    } else if ((inputBinary >= 7) && (inputBinary <= 8)) {
        inputBinary = 7;
    } else if ((inputBinary >= 9) && (inputBinary <= 15)) {
        inputBinary = 12;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bF(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 5;
    } else if ((inputBinary >= 7) && (inputBinary <= 9)) {
        inputBinary = 8;
    } else if ((inputBinary >= 10) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bG(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 5;
    } else if ((inputBinary >= 7) && (inputBinary <= 11)) {
        inputBinary = 9;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bH(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bH5(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 5) && (inputBinary <= 7)) {
        inputBinary = 5;
    } else if ((inputBinary >= 8) && (inputBinary <= 11)) {
        inputBinary = 10;
    } else if ((inputBinary >= 12) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bI(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 6) && (inputBinary <= 9)) {
        inputBinary = 8;
    } else if ((inputBinary >= 10) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bJ(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 6) && (inputBinary <= 10)) {
        inputBinary = 8;
    } else if ((inputBinary >= 11) && (inputBinary <= 15)) {
        inputBinary = 13;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bK(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 3) && (inputBinary <= 4)) {
        inputBinary = 3;
    } else if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 5;
    } else if ((inputBinary >= 7) && (inputBinary <= 9)) {
        inputBinary = 8;
    } else if ((inputBinary >= 10) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bK1(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 3) && (inputBinary <= 4)) {
        inputBinary = 3;
    } else if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 6;
    } else if ((inputBinary >= 7) && (inputBinary <= 9)) {
        inputBinary = 8;
    } else if ((inputBinary >= 10) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bK2(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 3) && (inputBinary <= 4)) {
        inputBinary = 3;
    } else if ((inputBinary >= 5) && (inputBinary <= 6)) {
        inputBinary = 6;
    } else if ((inputBinary >= 7) && (inputBinary <= 9)) {
        inputBinary = 9;
    } else if ((inputBinary >= 10) && (inputBinary <= 12)) {
        inputBinary = 11;
    } else if ((inputBinary >= 13) && (inputBinary <= 15)) {
        inputBinary = 14;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bL(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 2) && (inputBinary <= 3)) {
        inputBinary = 2;
    } else if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 4;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 6;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 13)) {
        inputBinary = 12;
    } else if ((inputBinary >= 14) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bL357(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 2) && (inputBinary <= 3)) {
        inputBinary = 3;
    } else if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 9;
    } else if ((inputBinary >= 11) && (inputBinary <= 13)) {
        inputBinary = 12;
    } else if ((inputBinary >= 14) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double NonUniQuan5bTo4bL3571013(double input)
{
    int sign = 1;
    int inputBinary;

    if (input < 0) {
        input *= -1;
        sign = -1;
    }
    
    inputBinary = (int)((input + DELTA) / RESOLUTION_V2C);

    if ((inputBinary >= 2) && (inputBinary <= 3)) {
        inputBinary = 3;
    } else if ((inputBinary >= 4) && (inputBinary <= 5)) {
        inputBinary = 5;
    } else if ((inputBinary >= 6) && (inputBinary <= 7)) {
        inputBinary = 7;
    } else if ((inputBinary >= 8) && (inputBinary <= 10)) {
        inputBinary = 10;
    } else if ((inputBinary >= 11) && (inputBinary <= 13)) {
        inputBinary = 13;
    } else if ((inputBinary >= 14) && (inputBinary <= 15)) {
        inputBinary = 15;
    }

    input = sign * inputBinary * RESOLUTION_V2C;
    return input;
}

double QuanChLLR(double input)
{
    int sign = 1;
    
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_CH_LLR - HALF_RESOLUTION_CH_LLR)) {
        input = MAX_CH_LLR;
    } else {
        input = ((int)((input + HALF_RESOLUTION_CH_LLR) / RESOLUTION_CH_LLR)) * RESOLUTION_CH_LLR;
    }

    return sign * input;
}

double QuanV2C(double input)
{
    int sign = 1;
    
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_V2C - HALF_RESOLUTION_V2C)) {
        input = MAX_V2C;
    } else {
        input = ((int)((input + HALF_RESOLUTION_V2C) / RESOLUTION_V2C)) * RESOLUTION_V2C;
    }

    return sign * input;
}

double QuanAPP(double input)
{
    int sign = 1;
    
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_APP - HALF_RESOLUTION_APP)) {
        input = MAX_APP;
    } else {
        input = ((int)((input + HALF_RESOLUTION_APP) / RESOLUTION_APP)) * RESOLUTION_APP;
    }

    return sign * input;
}

double SatuAppOrV2C(double input)
{
    if (input >= MAX_APP) {
        input = MAX_APP;
    } else if (input <= -MAX_APP) {
        input = -MAX_APP;
    }

    return input;
}

double QuanExtenAPP(double input)
{
    int sign = 1;
    
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input >= (MAX_EXTEN_APP - HALF_RESOLUTION_EXTEN_APP)) {
        input = MAX_EXTEN_APP;
    } else {
        input = ((int)((input + HALF_RESOLUTION_EXTEN_APP) / RESOLUTION_EXTEN_APP)) * RESOLUTION_EXTEN_APP;
    }

    return sign * input;
}



/*
double QuanChLLR(double input)
{
    int sign = 1;
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input < 1.8) {
        input = ((int)(input / 0.1)) * 0.1 + 0.1 / 2;
    } else {
        input = 10;
    }

    return sign * input;
}


double QuanC2V(double input)
{
    int sign = 1;
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input < 1.8) {
        input = ((int)(input / 0.1)) * 0.1 + 0.1 / 2;
    } else if (input < 4.2) {
        input = ((int)(input - 1.8) / 0.2) * 0.2 + 0.2 / 2 + 1.8;
    } else if (input < 6.6) {
        input = ((int)(input - 4.2) / 0.3) * 0.3 + 0.3 / 2 + 4.2;
    } else if (input < 9.0) {
        input = ((int)(input - 6.6) / 0.4) * 0.4 + 0.4 / 2 + 6.6;
    } else if (input < 10.0) {
        input = ((int)(input - 9.0) / 0.5) * 0.5 + 0.5 / 2 + 9.0;
    } else {
        input = 10;
    }

    return sign * input;
}

double QuanV2C(double input)
{
    int sign = 1;
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input < 1.8) {
        input = ((int)(input / 0.1)) * 0.1 + 0.1 / 2;
    } else if (input < 4.2) {
        input = ((int)(input - 1.8) / 0.2) * 0.2 + 0.2 / 2 + 1.8;
    } else if (input < 6.6) {
        input = ((int)(input - 4.2) / 0.3) * 0.3 + 0.3 / 2 + 4.2;
    } else if (input < 9.0) {
        input = ((int)(input - 6.6) / 0.4) * 0.4 + 0.4 / 2 + 6.6;
    } else if (input < 11.0) {
        input = ((int)(input - 9.0) / 0.5) * 0.5 + 0.5 / 2 + 9.0;
    } else if (input < 13.4) {
        input = ((int)(input - 11.0) / 0.6) * 0.6 + 0.6 / 2 + 11.0;
    } else if (input < 15.5) {
        input = ((int)(input - 13.4) / 0.7) * 0.7 + 0.7 / 2 + 13.4;
    } else if (input < 17.9) {
        input = ((int)(input - 15.5) / 0.8) * 0.8 + 0.8 / 2 + 15.5;
    } else if (input < 19.7) {
        input = ((int)(input - 17.9) / 0.9) * 0.9 + 0.9 / 2 + 17.9;
    } else if (input < 21.7) {
        input = ((int)(input - 19.7) / 1.0) * 1.0 + 1.0 / 2 + 19.7;
    } else if (input < 22.8) {
        input = ((int)(input - 21.7) / 1.1) * 1.1 + 1.1 / 2 + 21.7;
    } else {
        input = 23;
    }

    return sign * input;
}

double QuanAPP(double input)
{
    int sign = 1;
    if (input < 0) {
        input *= -1;
        sign = -1;
    }

    if (input < 1.8) {
        input = ((int)(input / 0.1)) * 0.1 + 0.1 / 2;
    } else if (input < 4.2) {
        input = ((int)(input - 1.8) / 0.2) * 0.2 + 0.2 / 2 + 1.8;
    } else if (input < 6.6) {
        input = ((int)(input - 4.2) / 0.3) * 0.3 + 0.3 / 2 + 4.2;
    } else if (input < 9.0) {
        input = ((int)(input - 6.6) / 0.4) * 0.4 + 0.4 / 2 + 6.6;
    } else if (input < 11.0) {
        input = ((int)(input - 9.0) / 0.5) * 0.5 + 0.5 / 2 + 9.0;
    } else if (input < 13.4) {
        input = ((int)(input - 11.0) / 0.6) * 0.6 + 0.6 / 2 + 11.0;
    } else if (input < 15.5) {
        input = ((int)(input - 13.4) / 0.7) * 0.7 + 0.7 / 2 + 13.4;
    } else if (input < 17.9) {
        input = ((int)(input - 15.5) / 0.8) * 0.8 + 0.8 / 2 + 15.5;
    } else if (input < 19.7) {
        input = ((int)(input - 17.9) / 0.9) * 0.9 + 0.9 / 2 + 17.9;
    } else if (input < 21.7) {
        input = ((int)(input - 19.7) / 1.0) * 1.0 + 1.0 / 2 + 19.7;
    } else if (input < 22.8) {
        input = ((int)(input - 21.7) / 1.1) * 1.1 + 1.1 / 2 + 21.7;
    } else {
        input = 23;
    }

    return sign * input;
}
*/


/*
#define MAX_V2C_APP (24.5)
#define MIN_V2C_APP (-MAX_V2C_APP)

#define MAX_C2V (9.5)
#define MIN_C2V (-MAX_C2V)

#define MAX_CH (9.5)
#define MIN_CH (-MAX_CH)

double ClipC2V(double input)
{
    if ((input <= MAX_C2V) && (input >= MIN_C2V)) {
        return input;
    } else if (input > MAX_C2V) {
        return MAX_C2V;
    } else {
        return MIN_C2V;
    }
}

double ClipCH(double input)
{
    if ((input <= MAX_CH) && (input >= MIN_CH)) {
        return input;
    } else if (input > MAX_CH) {
        return MAX_CH;
    } else {
        return MIN_CH;
    }
}

double ClipV2CAPP(double input)
{
    if ((input <= MAX_V2C_APP) && (input >= MIN_V2C_APP)) {
        return input;
    } else if (input > MAX_V2C_APP) {
        return MAX_V2C_APP;
    } else {
        return MIN_V2C_APP;
    }
}
*/
