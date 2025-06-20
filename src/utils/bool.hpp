#pragma once

// no std::vector<bool> pls
struct Bool {
  bool val = false;

  operator bool() const {
    return val;
  }
  Bool& operator=(bool b) {
    val = b;
    return *this;
  }
  Bool& operator&=(bool b) {
    val &= b;
    return *this;
  }
  bool operator&&(bool b) {
    return val && b;
  }
  Bool& operator|=(bool b) {
    val |= b;
    return *this;
  }
  bool operator||(bool b) {
    return val || b;
  }
  bool operator!() {
    return !val;
  }
  bool operator==(Bool const& other) {
    return val == other.val;
  }
};

