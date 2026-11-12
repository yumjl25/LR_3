use std::alloc::{alloc, realloc, dealloc, Layout};
use std::ptr;

const INIT_CAPACITY: usize = 4;

pub struct Array {
    data: *mut String,
    size: usize,
    capacity: usize,
}

impl Default for Array {
    fn default() -> Self {
        Self::new()
    }
}

impl Array {
    pub fn new() -> Self {
        let layout = Layout::array::<String>(INIT_CAPACITY).unwrap();
        let data = unsafe { alloc(layout) as *mut String };
        Array { data, size: 0, capacity: INIT_CAPACITY }
    }

    fn resize(&mut self) {
        let new_cap = self.capacity * 2;
        let old_layout = Layout::array::<String>(self.capacity).unwrap();
        let new_layout = Layout::array::<String>(new_cap).unwrap();
        unsafe {
            let new_data = realloc(self.data as *mut u8, old_layout, new_layout.size()) as *mut String;
            self.data = new_data;
            self.capacity = new_cap;
        }
    }

    pub fn push(&mut self, val: &str) {
        if self.size == self.capacity { self.resize(); }
        unsafe {
            ptr::write(self.data.add(self.size), val.to_string());
        }
        self.size += 1;
    }

    pub fn get(&self, index: usize) -> Option<&String> {
        if index >= self.size { return None; }
        unsafe { Some(&*self.data.add(index)) }
    }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        for i in 0..self.size {
            let s = unsafe { &*self.data.add(i) };
            writeln!(file, "{}", s).unwrap();
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if !line.is_empty() { self.push(line); }
            }
        }
    }

    pub fn len(&self) -> usize {
        self.size
    }

    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    pub fn del(&mut self, index: usize) -> Option<String> {
        if index >= self.size {
            return None;
        }

        unsafe {
            let removed = ptr::read(self.data.add(index));

            for i in index..self.size - 1 {
                let next_val = ptr::read(self.data.add(i + 1));
                ptr::write(self.data.add(i), next_val);
            }

            self.size -= 1;
            Some(removed)
        }
    }
}

impl Drop for Array {
    fn drop(&mut self) {
        unsafe {
            for i in 0..self.size { ptr::drop_in_place(self.data.add(i)); }
            let layout = Layout::array::<String>(self.capacity).unwrap();
            dealloc(self.data as *mut u8, layout);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_array() {
        let mut arr = Array::new();
        arr.push("A");
        arr.push("B");
        assert_eq!(arr.len(), 2);
        assert_eq!(arr.get(1).unwrap(), "B");

        arr.save_to_file("test_array.txt");
        let mut arr2 = Array::new();
        arr2.load_from_file("test_array.txt");
        assert_eq!(arr2.len(), 2);
        assert_eq!(arr2.get(0).unwrap(), "A");
    }

    #[test]
    fn test_array_del() {
        let mut arr = Array::new();
        arr.push("A");
        arr.push("B");
        arr.push("C");

        let removed = arr.del(1);
        assert_eq!(removed.unwrap(), "B");
        assert_eq!(arr.len(), 2);
        assert_eq!(arr.get(0).unwrap(), "A");
        assert_eq!(arr.get(1).unwrap(), "C");

        let mut empty = Array::new();
        assert!(empty.del(0).is_none());
    }
}