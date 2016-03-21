#ifndef SKP_LOCAL_STATUS_H
#define SKP_LOCAL_STATUS_H

typedef enum {
    LocalStatusClassSuccess = 0,
    LocalStatusClassError,
    LocalStatusClassMemoryFile,
    LocalStatusClassLogin,
}LocalStatusClass_;

typedef enum {
    LocalStatusTypeMemoryFile = 0,
    LocalStatusTypeMemoryFileMD5,
    LocalStatusTypeMemoryFileData,
    LocalStatusTypeMemoryFileSeek,
    LocalStatusTypeMemoryFilePushing,
    LocalStatusTypeMemoryFileFinish,
}LocalStatusTypeMemoryFile_;

typedef enum {
    LocalStatusTypeLogin = 0,
    LocalStatusTypeLoginUser,
    LocalStatusTypeLoginPassword,
}LocalStatusTypeLogin_;

#endif // SKP_LOCAL_STATUS_H
