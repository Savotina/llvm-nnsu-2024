// RUN: %clang_cc1 -load %llvmshlibdir/AddAttrAlwaysInlinePlugin%pluginext\
// RUN: -plugin add_attr_always_inline_plugin %s 1>&1 | FileCheck %s

namespace {
// CHECK: function: square
// CHECK: attr status (always_inline): false
// CHECK: function: square
// CHECK: attr status (always_inline): true
// CHECK: ==================================
int square(int value) { return value * value; }

// CHECK: function: diff
// CHECK: attr status (always_inline): false
// CHECK: function: diff
// CHECK: attr status (always_inline): true
// CHECK: ==================================
int diff(int valueOne, int valueTwo) {
  {} {} {{} {}} {} {{{}} {} {}} {} {
    { return valueOne - valueTwo; }
  }
}

// CHECK: function: max
// CHECK: attr status (always_inline): false
// CHECK: function: max
// CHECK: attr status (always_inline): true
// CHECK: ==================================
template <typename T> T max(T valueOne, T valueTwo) {
  return valueOne > valueTwo ? valueOne : valueTwo;
}

// CHECK: function: emptyFunc
// CHECK: attr status (always_inline): true
// CHECK: function: emptyFunc
// CHECK: attr status (always_inline): true
// CHECK: ==================================
__attribute__((always_inline)) void emptyFunc() {
  {} {{}} {{} {} {{{{}}}}} {}
}

// CHECK: function: funcTestIfStmt
// CHECK: attr status (always_inline): false
// CHECK: function: funcTestIfStmt
// CHECK: attr status (always_inline): false
// CHECK: ==================================
bool funcTestIfStmt(int value) {
  if (value % 2)
    return false;
  return true;
}

// CHECK: function: funcTestSwitchStmt
// CHECK: attr status (always_inline): false
// CHECK: function: funcTestSwitchStmt
// CHECK: attr status (always_inline): false
// CHECK: ==================================
int funcTestSwitchStmt(int value) {
  switch (value) {
  case 1:
    return value;
  case 2:
    return value;
  case 3:
    return value;
  default:
    return value;
  }
}

// CHECK: function: funcTestWhileStmt
// CHECK: attr status (always_inline): false
// CHECK: function: funcTestWhileStmt
// CHECK: attr status (always_inline): false
// CHECK: ==================================
void funcTestWhileStmt(int value) {
  {
    while (value--) {
    }
  }
  {} {}
}

// CHECK: function: funcTestDoStmt
// CHECK: attr status (always_inline): false
// CHECK: function: funcTestDoStmt
// CHECK: attr status (always_inline): false
// CHECK: ==================================
void funcTestDoStmt(int value) {
  do {
    --value;
  } while (value);
}

// CHECK: function: funcTestForStmt
// CHECK: attr status (always_inline): false
// CHECK: function: funcTestForStmt
// CHECK: attr status (always_inline): false
// CHECK: ==================================
void funcTestForStmt(unsigned value) {
  for (unsigned i = 0; i < value; ++i) {
  }
}
} // namespace
