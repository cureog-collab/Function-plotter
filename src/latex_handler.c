#include "../include/main.h"

#include <string.h>

typedef struct {
    const char *latexSymbol;
    char *cSymbol;
    int latexSymLen;
    int cSymLen;
} latexMap;

latexMap latexToCDict[] = {
    // 1. simple operators
    {"\\cdot", "*", 5, 1},
    {"\\times", "*", 6, 1},
    {"\\div", "/", 4, 1},
    
    // 2. square root
    {"\\sqrt", "sqrt", 5, 4},
    
    // 3. logarits and exponentials
    {"\\log", "log10", 4, 5},
    {"\\ln", "log", 3, 3},
    {"\\exp", "exp", 4, 3},
    
    // 4. trig
    {"\\sin", "sin", 4, 3},
    {"\\cos", "cos", 4, 3},
    {"\\tan", "tan", 4, 3},
    {"\\cot", "1.0/tan", 4, 7},
    
    // 5. inverser trig
    {"\\arcsin", "asin", 7, 4},
    {"\\arccos", "acos", 7, 4},
    {"\\arctan", "atan", 7, 4},
    
    // 6. hyperbolic
    {"\\sinh", "sinh", 5, 4},
    {"\\cosh", "cosh", 5, 4},
    {"\\tanh", "tanh", 5, 4},
    
    // 7. constants
    {"\\pi", "pi", 3, 2}
};
const int countOfLatexToCDict = sizeof(latexToCDict) / sizeof(latexToCDict[0]);

void phaseOneConvertLatexToC(char *src, char *dst);
bool phaseTwoConvertLatexToC(char *src, char *dst, int *srcIdx, int *dstIdx, char endOfString);

bool convertLatexToC(char *src, char *dst)
{
    char bufferPhaseOne[2048];
    phaseOneConvertLatexToC(src, bufferPhaseOne);

    int phase2SrcIdx = 0;
    int phase2DstIdx = 0;

    if (!phaseTwoConvertLatexToC(bufferPhaseOne, dst, &phase2SrcIdx, &phase2DstIdx, '\0'))
    {
        printf("Error: cannot read the Latex input!\n");
        return false;
    }

    dst[phase2DstIdx] = '\0';

    // detect if the final form contains anything like "y=...", "u=..." and cut that part out
    char *equalSign = strrchr(dst, '=');
    if (equalSign != NULL)
    {
        char *actualExpr = equalSign + 1;
        int exprLen = strlen(actualExpr) + 1;
        memmove(dst, actualExpr, exprLen);
    }
    
    return true;
}

// convert simple operators (sin, cos, times, cdot, div,...) and remove blank spaces
void phaseOneConvertLatexToC(char *src, char *dst)
{
    int srcIdx = 0;
    int dstIdx = 0;

    while (src[srcIdx] != '\0')
    {
        switch (src[srcIdx])
        {
            case '\\':
            {
                bool foundMath = false;
                for (int i = 0; i < countOfLatexToCDict; ++i)
                {
                    char *expression = src + srcIdx;
                    if (strncmp(expression, latexToCDict[i].latexSymbol, latexToCDict[i].latexSymLen) == 0)
                    {
                        foundMath = true;
                        strcpy(&dst[dstIdx], latexToCDict[i].cSymbol);
                        dstIdx += latexToCDict[i].cSymLen;
                        srcIdx += latexToCDict[i].latexSymLen;
                        break;
                    }
                }

                if (!foundMath)
                {
                    dst[dstIdx] = src[srcIdx];
                    srcIdx++;
                    dstIdx++;
                }
                break;
            }

            case ' ':
            {
                srcIdx++;
                break;
            }

            default:
            {
                dst[dstIdx] = src[srcIdx];
                srcIdx++;
                dstIdx++;
                break;
            }
        }
    }
    dst[dstIdx] = '\0';
}

// convert so-called "structural opperators" (\frac{}{}, ^, !, ||...) and handle invalid math input
bool phaseTwoConvertLatexToC(char *src, char *dst, int *srcIdx, int *dstIdx, char endOfString)
{
    while (src[*srcIdx] && src[*srcIdx] != endOfString)
    {
        switch (src[*srcIdx])
        {
            case '\\':
            {
                bool foundMath = false;

                // \frac{}{}
                if (strncmp(src + *srcIdx, "\\frac", 5) == 0)
                {
                    *srcIdx += 6;
                    dst[*dstIdx] = '(';
                    (*dstIdx)++;

                    // recursive call for the numerator
                    if (phaseTwoConvertLatexToC(src, dst, srcIdx, dstIdx, '}'))
                    {
                        dst[*dstIdx] = ')';
                        dst[*dstIdx + 1] = '/';
                        dst[*dstIdx + 2] = '(';
                        *dstIdx += 3;
                        *srcIdx += 2;

                        if (!phaseTwoConvertLatexToC(src, dst, srcIdx, dstIdx, '}'))
                        {
                            return false; 
                        }

                        // recursive call for the denominator
                        if (phaseTwoConvertLatexToC(src, dst, srcIdx, dstIdx, '}'))
                        {
                            dst[*dstIdx] = ')';
                            (*dstIdx)++;
                            (*srcIdx)++;
                        }
                    }
                    foundMath = true;
                }

                if (!foundMath)
                {
                    printf("Error: invalid operator!\n");
                    return false;
                }
                break;
            }

            case '{':
            {
                dst[*dstIdx] = '(';
                (*dstIdx)++;
                (*srcIdx)++;

                if (!phaseTwoConvertLatexToC(src, dst, srcIdx, dstIdx, '}'))
                {
                    return false;
                }

                dst[*dstIdx] = ')';
                (*dstIdx)++;
                (*srcIdx)++;
                break;
            }

            default:
            {
                if (src[*srcIdx] == '}')
                {
                    dst[*dstIdx] = ')';
                }
                else 
                {
                    dst[*dstIdx] = src[*srcIdx];
                }

                (*srcIdx)++;
                (*dstIdx)++;
                break;
            }
        }
    }

    return true;
}

// int findCloseBracket(char *str, int openIdx)
// {
//     int depth = 0;
//     int i = 0;
//     while (str[openIdx + i] != '\0')
//     {
//         switch (str[openIdx + i])
//         {
//             case '{':
//             {
//                 depth++;
//                 i++;
//                 break;
//             }

//             case '}':
//             {
//                 depth--;
//                 i++;
//                 break;
//             }
//         }

//         if (depth == 0)
//         {
//             return openIdx + i;
//         }
//     }

//     return -1;
// }