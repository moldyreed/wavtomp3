#include "filesystem.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

std::vector<std::string> filesystem::getFilesByPath(const std::string& path)
{
    std::vector<std::string> files;
    if(isDirectory(path))
    {

        char filename_qfd[100] ;
        struct dirent *dp;
        DIR *dfd;
        auto dir = path.c_str();

        if ((dfd = opendir(dir)) == NULL)
         {
          throw std::logic_error("Can't open");

         }

        while ((dp = readdir(dfd)) != NULL)
         {
          struct stat stbuf{0};
          sprintf( filename_qfd , "%s/%s",dir,dp->d_name) ;
          if( stat(filename_qfd,&stbuf ) == -1 )
          {
           printf("Unable to stat file: %s\n",filename_qfd) ;
           continue ;
          }

          if ( ( stbuf.st_mode & S_IFMT ) == S_IFDIR )
          {
           continue;
           // Skip directories
          }
          else
          {
             files.emplace_back(filename_qfd);
          }
         }
    }
    else
        throw std::logic_error("Not a directory");
    return files;
}

bool filesystem::isDirectory(const std::string& path)
{
    struct stat pathStat{0};
    if(stat(path.c_str(), &pathStat) != 0)
    {
        throw std::logic_error("Directory not exists");
    }
    return S_ISDIR(pathStat.st_mode);
}
