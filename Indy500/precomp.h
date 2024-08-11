#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/audio/Voice.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ip/Flip.h"
