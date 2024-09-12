#ifndef REF_COUNTED_H
#define REF_COUNTED_H

class RefCounted {
public:
  RefCounted() : refCount(0) {}

  void addRef() { ++refCount; }

  void release() {
    if (--refCount == 0) {
      delete this;
    }
  }

private:
  int refCount;
};

#endif // REF_COUNTED_H