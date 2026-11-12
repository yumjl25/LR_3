use std::rc::Rc;
use std::cell::RefCell;

pub type Link = Option<Rc<RefCell<Node>>>;

pub struct Node {
    pub data: String,
    pub next: Link,
}

pub struct ForwardList {
    head: Link,
    tail: Link,
    size: usize,
}

impl Default for ForwardList {
    fn default() -> Self {
        Self::new()
    }
}

impl ForwardList {
    pub fn new() -> Self {
        ForwardList { head: None, tail: None, size: 0 }
    }

    pub fn push_head(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(Node { data: val.to_string(), next: self.head.clone() }));
        if self.tail.is_none() {
            self.tail = Some(Rc::clone(&node));
        }
        self.head = Some(node);
        self.size += 1;
    }

    pub fn push_tail(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(Node { data: val.to_string(), next: None }));
        match &self.tail {
            Some(t) => t.borrow_mut().next = Some(Rc::clone(&node)),
            None => self.head = Some(Rc::clone(&node)),
        }
        self.tail = Some(node);
        self.size += 1;
    }

    pub fn push_before(&mut self, target: &str, val: &str) {
        if let Some(head) = self.head.clone() {
            if head.borrow().data == target {
                self.push_head(val);
                return;
            }
        }
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            let next = node.borrow().next.clone();
            if let Some(n) = next {
                if n.borrow().data == target {
                    let new_node = Rc::new(RefCell::new(Node { data: val.to_string(), next: Some(Rc::clone(&n)) }));
                    node.borrow_mut().next = Some(new_node);
                    self.size += 1;
                    return;
                }
            }
            cur = node.borrow().next.clone();
        }
        println!("Target not found");
    }

    pub fn push_after(&mut self, target: &str, val: &str) {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target {
                let new_node = Rc::new(RefCell::new(Node {
                    data: val.to_string(),
                    next: node.borrow().next.clone(),
                }));
                node.borrow_mut().next = Some(Rc::clone(&new_node));
                if let Some(tail) = &self.tail {
                    if Rc::ptr_eq(tail, &node) {
                        self.tail = Some(new_node);
                    }
                }
                self.size += 1;
                return;
            }
            cur = node.borrow().next.clone();
        }
        println!("Target not found");
    }

    pub fn del_head(&mut self) -> Option<String> {
        self.head.take().map(|head| {
            let val = head.borrow().data.clone();
            let next = head.borrow().next.clone();
            self.head = next;
            if self.head.is_none() {
                self.tail = None;
            }
            self.size -= 1;
            val
        })
    }

    pub fn del_tail(&mut self) -> Option<String> {
        self.head.as_ref()?;
        if Rc::ptr_eq(self.head.as_ref().unwrap(), self.tail.as_ref().unwrap()) {
            return self.del_head();
        }
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            let next = node.borrow().next.clone();
            if let Some(n) = next {
                if let Some(tail) = &self.tail {
                    if Rc::ptr_eq(&n, tail) {
                        let val = n.borrow().data.clone();
                        node.borrow_mut().next = None;
                        self.tail = Some(Rc::clone(&node));
                        self.size -= 1;
                        return Some(val);
                    }
                }
                cur = Some(n);
            } else {
                break;
            }
        }
        None
    }

    pub fn del_value(&mut self, target: &str) -> bool {
        if let Some(head) = self.head.clone() {
            if head.borrow().data == target {
                self.del_head();
                return true;
            }
        }
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            let next = node.borrow().next.clone();
            if let Some(n) = next {
                if n.borrow().data == target {
                    let after = n.borrow().next.clone();
                    node.borrow_mut().next = after;
                    if let Some(tail) = &self.tail {
                        if Rc::ptr_eq(tail, &n) {
                            self.tail = Some(Rc::clone(&node));
                        }
                    }
                    self.size -= 1;
                    return true;
                }
                cur = Some(n);
            } else {
                break;
            }
        }
        false
    }

    pub fn search(&self, target: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target { return true; }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn get(&self, index: usize) -> Option<String> {
        if index >= self.size { return None; }
        let mut cur = self.head.clone();
        for _ in 0..index {
            if let Some(node) = cur {
                cur = node.borrow().next.clone();
            }
        }
        cur.map(|n| n.borrow().data.clone())
    }

    pub fn get_head(&self) -> Option<String> {
        self.head.as_ref().map(|n| n.borrow().data.clone())
    }

    pub fn get_tail(&self) -> Option<String> {
        self.tail.as_ref().map(|n| n.borrow().data.clone())
    }

    pub fn len(&self) -> usize { self.size }
    pub fn is_empty(&self) -> bool { self.size == 0 }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            writeln!(file, "{}", node.borrow().data).unwrap();
            cur = node.borrow().next.clone();
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if !line.is_empty() { self.push_tail(line); }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_flist_full() {
        let mut fl = ForwardList::new();
        assert!(fl.is_empty());

        fl.push_head("B");
        fl.push_head("A"); // A, B
        fl.push_tail("D");
        fl.push_before("D", "C"); // A, B, C, D
        fl.push_after("D", "E");  // A, B, C, D, E

        assert_eq!(fl.len(), 5);
        assert_eq!(fl.get_head().unwrap(), "A");
        assert_eq!(fl.get_tail().unwrap(), "E");
        assert_eq!(fl.get(2).unwrap(), "C");

        assert!(fl.search("C"));
        assert!(!fl.search("Z"));

        assert_eq!(fl.del_head().unwrap(), "A");
        assert_eq!(fl.del_tail().unwrap(), "E");
        assert!(fl.del_value("C"));
        assert!(!fl.del_value("Z"));
        assert_eq!(fl.len(), 2);

        fl.push_before("none", "val");
        fl.push_after("none", "val");
        assert_eq!(fl.get(10), None);

        fl.save_to_file("test_flist.txt");
        let mut fl2 = ForwardList::new();
        fl2.load_from_file("test_flist.txt");
        assert_eq!(fl2.len(), 2);
    }
}