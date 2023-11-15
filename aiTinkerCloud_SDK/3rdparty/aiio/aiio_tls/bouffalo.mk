# Component Makefile
#
## These include paths would be exported to project level
COMPONENT_ADD_INCLUDEDIRS += . \
                             aiio-tls-crypto

## not be exported to project level
COMPONENT_PRIV_INCLUDEDIRS := private_include

## This component's src
COMPONENT_SRCS += aiio_tls.c                       \
                  aiio-tls-crypto/aiio_tls_crypto.c \
                  aiio_tls_error_capture.c         \
                  aiio_tls_mbedtls.c               \


COMPONENT_OBJS := $(patsubst %.c,%.o, $(COMPONENT_SRCS))

COMPONENT_SRCDIRS := . \
                     aiio-tls-crypto

##
#CPPFLAGS +=
