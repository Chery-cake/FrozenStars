module;

export module memory;

export import memory.allocator;

// TODO verify the use of this implementation
// std::pmr already implements most of the allocators
// it doesn't have a stack allocator, maybe that's the only one needing
// impllementaion
