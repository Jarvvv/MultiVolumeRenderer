//
// Created by wyz on 20-12-11.
//

#ifndef VOLUMERENDERER_HELP_GL_H
#define VOLUMERENDERER_HELP_GL_H

#define DEBUG
#ifdef DEBUG
#define GL_EXPR(exec) \
        {             \
            GLenum gl_err; \
            exec;     \
            if((gl_err=glGetError())!=GL_NO_ERROR){ \
                std::cout<<#exec<<" on line "<<__LINE__<<" of "<<__FILE__<<" caused GL error: "<< \
                static_cast<unsigned>(gl_err)<<std::endl;                                         \
            }\
        };

#define GL_CHECK \
         {       \
            GLenum gl_err; \
            if((gl_err=glGetError())!=GL_NO_ERROR){     \
                std::cout<<"GL error: "<<static_cast<unsigned>(gl_err)<<" caused before line "<< \
                __LINE__<<" in file "<<__FILE__<<std::endl; \
            }\
         }

#else
#define GL_EXPR(exec) exec
#define GL_CHECK
#endif

#endif //VOLUMERENDERER_HELP_GL_H
