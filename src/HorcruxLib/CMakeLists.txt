project(HorcruxLib)

set(HorcruxLib_src
Horcrux.cpp
HorcruxAes.h
HorcruxAes.cpp
stdafx.h
stdafx.cpp
Utils.h
)

add_library(HorcruxLib STATIC ${HorcruxLib_src}) ## PRIMA NO NERA STATICA