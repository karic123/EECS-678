#ifndef _RACHELS_UTILS_HPP
#define _RACHELS_UTILS_HPP

/*
    write               read
    output <<< PIPE <<< input
*/

const int INPUT_PIPE 	= 0;
const int OUTPUT_PIPE 	= 1;

const int READ_PIPE 	= 0;
const int WRITE_PIPE 	= 1;

const int CHILD_PROCESS = 0;
const int FORK_ERROR = -1;

char* GetExecvError( int code )
{
    switch( code )
    {
        case ENODEV:    return "ENODEV		The device prefix of program did not exist.";    break;
        case ENOTDIR:   return "ENOTDIR		A non-final component of program was not a directory.";    break;
        case ENOENT:    return "ENOENT		program did not exist.";    break;
        case EISDIR:    return "EISDIR		program is a directory.";    break;
        case ENOEXEC:   return "ENOEXEC		program is not in a recognizable executable file format, was for the wrong platform, or contained invalid fields.";    break;
        case ENOMEM:    return "ENOMEM		Insufficient virtual memory is available.";    break;
        case E2BIG:     return "E2BIG		The total size of the argument strings is too large.";    break;
        case EIO:       return "EIO		A hard I/O error occurred.";    break;
        case EFAULT:    return "EFAULT		One of the args is an invalid pointer.";    break;
        default:        return "Unknown error code"; break;

    }
}

#endif
