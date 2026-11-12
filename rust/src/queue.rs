pub struct QNode {
    pub data: String,
    pub next: Option<Box<QNode>>,
}

pub struct Queue {
    head: Option<Box<QNode>>,
    tail: Option<*mut QNode>,
    size: usize,
}

impl Default for Queue {
    fn default() -> Self {
        Self::new()
    }
}

impl Queue {
    pub fn new() -> Self {
        Queue { head: None, tail: None, size: 0 }
    }

    pub fn push(&mut self, val: &str) {
        let mut node = Box::new(QNode { data: val.to_string(), next: None });
        let raw_ptr: *mut QNode = &mut *node;

        match self.tail {
            None => {
                self.head = Some(node);
                self.tail = Some(raw_ptr);
            }
            Some(tail_ptr) => {
                unsafe {
                    (*tail_ptr).next = Some(node);
                }
                self.tail = Some(raw_ptr);
            }
        }
        self.size += 1;
    }

    pub fn pop(&mut self) -> Option<String> {
        self.head.take().map(|node| {
            self.head = node.next;
            if self.head.is_none() {
                self.tail = None;
            }
            self.size -= 1;
            node.data
        })
    }

    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        let mut cur = &self.head;
        while let Some(node) = cur {
            writeln!(file, "{}", node.data).unwrap();
            cur = &node.next;
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if !line.is_empty() {
                    self.push(line);
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_queue_basic() {
        let mut q = Queue::new();
        q.push("first");
        q.push("second");
        q.push("third");

        assert_eq!(q.size, 3);
        assert_eq!(q.pop().unwrap(), "first");
        assert_eq!(q.pop().unwrap(), "second");
        assert_eq!(q.pop().unwrap(), "third");
        assert!(q.is_empty());
    }

    #[test]
    fn test_queue_serialization() {
        let mut q = Queue::new();
        q.push("A");
        q.push("B");
        q.save_to_file("test_queue.txt");

        let mut q2 = Queue::new();
        q2.load_from_file("test_queue.txt");
        assert_eq!(q2.size, 2);
        assert_eq!(q2.pop().unwrap(), "A");
    }
}