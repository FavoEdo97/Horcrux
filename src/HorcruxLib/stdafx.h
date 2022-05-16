#pragma once
#include <iostream>
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <vector>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <fstream>
#include <filesystem>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include "Utils.h"
#include "Horcrux.h"
#include "HorcruxAes.h"