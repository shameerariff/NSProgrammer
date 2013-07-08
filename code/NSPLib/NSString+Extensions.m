/*
 
 Copyright (C) 2013 Nolan O'Brien
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#import "NSString+Extensions.h"
#import "NSPStringUtils.h"

@implementation NSString (Extensions)

- (unsigned long long) unsignedLongLongValue
{
    if (self.length == 0)
    {
        return 0;
    }

    unsigned long long result = 0;
    const char*        cStr   = self.UTF8String;
    const char*        cCur   = cStr;
    unsigned long long newResult;

    while (isWhitespaceCharacter(*cCur))
        cCur++;

    // go through all decimal digit characters and terminates at first non digit
    // can be the NULL char string terminator
    while (isDecimalCharacter(*cCur))
    {
        newResult = result * 10;
        if (newResult < result)
        {
            return ULLONG_MAX; // overflow
        }
        newResult += (*cCur - '0');
        result     = newResult;
        ++cCur;
    }

    return result;
}

@end
