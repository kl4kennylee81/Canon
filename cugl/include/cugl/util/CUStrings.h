//
//  CUStrings.h
//  Cornell University Game Library (CUGL)
//
//  Android does not support a lot of the built-in string methods.  Therefore,
//  we need alternate definitions that are platform agnostic.  Note that these
//  functions have names that are very similar to those in the std namespace,
//  but all live in the cocos2d namespace.
//
//  Note that this module does not refer to the integral types as short, int,
//  long, etc.  Those types are NOT cross-platform.  For example, a long is
//  8 bytes on Unix/OS X, but 4 bytes on some Win32 platforms.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 2/10/16
//
#ifndef CU_STRINGS_H
#define CU_STRINGS_H
#include <SDL/SDL.h>
#include <string>

namespace cugl {
    
#pragma mark NUMBER TO STRING FUNCTIONS
    /**
     * Returns a string equivalent to the given byte
     *
     * The value is displayed as a number, not a character.
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given byte
     */
    std::string to_string(Uint8 value);
    
    /**
     * Returns a string equivalent to the given signed 16 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given signed 16 bit integer
     */
    std::string to_string(Sint16 value);
    
    /**
     * Returns a string equivalent to the given unsigned 16 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given unsigned 16 bit integer
     */
    std::string to_string(Uint16 value);
    
    /**
     * Returns a string equivalent to the given signed 32 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given signed 32 bit integer
     */
    std::string to_string(Sint32 value);
    
    /**
     * Returns a string equivalent to the given unsigned 32 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given unsigned 32 bit integer
     */
    std::string to_string(Uint32 value );
    
    /**
     * Returns a string equivalent to the given signed 64 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given signed 64 bit integer
     */
    std::string to_string(Sint64 value );
    
    /**
     * Returns a string equivalent to the given unsigned 64 bit integer
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given unsigned 64 bit integer
     */
    std::string to_string(Uint64 value );
    
    /**
     * Returns a string equivalent to the given float value
     *
     * The precision is the same as std::to_string(float).
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given float value
     */
    std::string to_string(float value);
    
    /**
     * Returns a string equivalent to the given double value
     *
     * The precision is the same as std::to_string(double).
     *
     * @param  value    the numeric value to convert
     *
     * @return a string equivalent to the given double value
     */
    std::string to_string(double value);

    
#pragma mark -
#pragma mark ARRAY TO STRING FUNCTIONS
    /**
     * Returns a string equivalent to the given byte array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the given byte array
     */
    std::string to_string(Uint8* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the signed 16 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the signed 16 bit integer array
     */
    std::string to_string(Sint16* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the unsigned 16 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the unsigned 16 bit integer array
     */
    std::string to_string(Uint16* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the signed 32 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the signed 32 bit integer array
     */
    std::string to_string(Sint32* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the unsigned 32 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the unsigned 32 bit integer array
     */
    std::string to_string(Uint32* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the signed 64 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the signed 64 bit integer array
     */
    std::string to_string(Sint64* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the unsigned 64 bit integer array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the unsigned 64 bit integer array
     */
    std::string to_string(Uint64* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the given float array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the given float array
     */
    std::string to_string(float* array, size_t length, size_t offset=0);
    
    /**
     * Returns a string equivalent to the given double array
     *
     * The value is display as a python-style list in brackets.
     *
     * @param array     the array to convert
     * @param length    the array length
     * @param offset    the starting position in the array
     *
     * @return a string equivalent to the given double array
     */
    std::string to_string(double* array, size_t length, size_t offset=0);
    
    
#pragma mark -
#pragma mark STRING TO NUMBER FUNCTIONS
    /**
     * Returns the byte equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to an integer value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the byte equivalent to the given string
     */
    Uint8 stou8(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the signed 16 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the signed 16 bit integer equivalent to the given string
     */
    Sint16 stos16(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the unsigned 16 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the unsigned 16 bit integer equivalent to the given string
     */
    Uint16 stou16(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the signed 32 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the signed 32 bit integer equivalent to the given string
     */
    Sint32 stos32(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the unsigned 32 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the unsigned 32 bit integer equivalent to the given string
     */
    Uint32 stou32(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the signed 64 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the signed 64 bit integer equivalent to the given string
     */
    Sint64 stos64(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the unsigned 64 bit integer equivalent to the given string
     *
     * This function discards any whitespace characters (as identified by calling isspace())
     * until the first non-whitespace character is found, then takes as many characters
     * as possible to form a valid base-n (where n=base) integer number representation
     * and converts them to a long value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     * @param  base the number base
     *
     * @return the unsigned 64 bit integer equivalent to the given string
     */
    Uint64 stou64(const std::string& str, std::size_t* pos = 0, int base = 10);
    
    /**
     * Returns the float equivalent to the given string
     *
     * This function discards any whitespace characters (as determined by std::isspace())
     * until first non-whitespace character is found. Then it takes as many characters as
     * possible to form a valid floating point representation and converts them to a floating
     * point value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     *
     * @return the float equivalent to the given string
     */
    float stof(const std::string& str, std::size_t* pos = 0);
    
    /**
     * Returns the double equivalent to the given string
     *
     * This function discards any whitespace characters (as determined by std::isspace())
     * until first non-whitespace character is found. Then it takes as many characters as
     * possible to form a valid floating point representation and converts them to a floating
     * point value.
     *
     * @param  str  the string to convert
     * @param  pos  address of an integer to store the number of characters processed
     *
     * @return the double equivalent to the given string
     */
    double stod(const std::string& str, std::size_t* pos = 0);
    
}

#endif /* CU_STRINGS_H */
