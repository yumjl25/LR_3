struct SNode {
    data: String,
    next: Option<Box<SNode>>,
}

pub struct Stack {
    head: Option<Box<SNode>>,
    size: usize,
}

impl Default for Stack {
    fn default() -> Self {
        Self::new()
    }
}

impl Stack {
    pub fn new() -> Self { Stack { head: None, size: 0 } }

    pub fn push(&mut self, val: &str) {
        let node = Box::new(SNode { data: val.to_string(), next: self.head.take() });
        self.head = Some(node);
        self.size += 1;
    }

    pub fn pop(&mut self) -> Option<String> {
        self.head.take().map(|node| {
            self.head = node.next;
            self.size -= 1;
            node.data
        })
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
            let lines: Vec<&str> = content.lines().collect();
            for line in lines.iter().rev() {
                if !line.is_empty() { self.push(line); }
            }
        }
    }

    pub fn is_empty(&self) -> bool {
        self.head.is_none()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_stack() {
        let mut s = Stack::new();
        s.push("first");
        s.push("second");
        assert_eq!(s.pop().unwrap(), "second");
        assert_eq!(s.pop().unwrap(), "first");
        assert!(s.is_empty());

        s.push("test");
        s.save_to_file("test_stack.txt");
        let mut s2 = Stack::new();
        s2.load_from_file("test_stack.txt");
        assert_eq!(s2.pop().unwrap(), "test");
    }
}