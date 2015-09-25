APP_PLATFORM := android-10
APP_STL := gnustl_static
APP_CFLAGS += -Wno-psabi
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -fsigned-char -std=gnu++11 -D__STDC_INT64__ -Wno-error=format-security -fexceptions
APP_LDFLAGS := -latomic
NDK_TOOLCHAIN_VERSION := 4.8
ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif

