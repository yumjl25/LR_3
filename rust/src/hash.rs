const TABLE_SIZE: usize = 1000;

struct HashNode {
    key: String,
    value: String,
    next: Option<Box<HashNode>>,
}

pub struct HashTable {
    buckets: [Option<Box<HashNode>>; TABLE_SIZE],
    size: usize,
}

impl Default for HashTable {
    fn default() -> Self { Self::new() }
}

impl HashTable {
    pub fn new() -> Self {
        let buckets = std::array::from_fn(|_| None);
        HashTable { buckets, size: 0 }
    }

    fn hash(&self, key: &str) -> usize {
        let mut h = 0;
        for c in key.chars() { h = (h + c as usize) % TABLE_SIZE; }
        h
    }

    pub fn add(&mut self, key: &str, value: &str) {
        let idx = self.hash(key);
        let mut cur = &mut self.buckets[idx];
        
        while let Some(node) = cur {
            if node.key == key {
                node.value = value.to_string();
                return;
            }
            cur = &mut node.next;
        }
        
        let new_node = Box::new(HashNode { key: key.to_string(), value: value.to_string(), next: self.buckets[idx].take() });
        self.buckets[idx] = Some(new_node);
        self.size += 1;
    }

    pub fn get(&self, key: &str) -> Option<&String> {
        let idx = self.hash(key);
        let mut cur = &self.buckets[idx];
        while let Some(node) = cur {
            if node.key == key { return Some(&node.value); }
            cur = &node.next;
        }
        None
    }

    pub fn del(&mut self, key: &str) -> bool {
        let idx = self.hash(key);
        let mut cur = &mut self.buckets[idx];
        while cur.is_some() {
            if cur.as_ref().unwrap().key == key {
                let mut node = cur.take().unwrap();
                *cur = node.next.take();
                self.size -= 1;
                return true;
            }
            cur = &mut cur.as_mut().unwrap().next;
        }
        false
    }

    pub fn len(&self) -> usize { self.size }
    pub fn is_empty(&self) -> bool { self.size == 0 }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        for i in 0..TABLE_SIZE {
            let mut cur = &self.buckets[i];
            while let Some(node) = cur {
                writeln!(file, "{}:{}", node.key, node.value).unwrap();
                cur = &node.next;
            }
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        *self = Self::new();
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if let Some((k, v)) = line.split_once(':') { self.add(k, v); }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hash_full() {
        let mut ht = HashTable::new();
        assert!(ht.is_empty());

        ht.add("apple", "10");
        ht.add("banana", "20");
        assert_eq!(ht.get("apple").unwrap(), "10");
        assert_eq!(ht.get("banana").unwrap(), "20");

        ht.add("apple", "99"); 
        assert_eq!(ht.get("apple").unwrap(), "99");
        assert_eq!(ht.len(), 2);

        assert!(ht.del("apple"));
        assert!(!ht.del("none"));
        assert_eq!(ht.get("apple"), None);

        ht.save_to_file("test_hash.txt");
        let mut ht2 = HashTable::new();
        ht2.load_from_file("test_hash.txt");
        assert_eq!(ht2.get("banana").unwrap(), "20");
    }
}