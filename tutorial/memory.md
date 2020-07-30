# Memory management, ownership and references

The json-c library makes memory management somewhat easier than traditional C as a JSON object is owned by either the programmer or another JSON object. A reference count for each JSON object is stored and incremented or decremented by certain json-c functions (get and put).  The JSON object's memory will be freed when its reference count reaches zero. Freeing a JSON object also frees the memory of all the objects the JSON object owns. So in addition to reference counts, A JSON object is either owned by the programmer or another JSON object. Certain json-c functions change the ownership of an object. If more than one JSON object owns a given JSON object, then that object's reference count is a count of all the objects which own it. The only objects programmers need to explicitly manage the memory of in json-c are the JSON objects they own.

This entire discussion is to clarify these issues and answer the question of when to call _*json_object_put*_ or _*json_object_get*_. This issue can be somewhat confusing to new users of the json-c library. Currently, there is no centralized discussion of this in the json-c documentation, instead, the issue is mentioned in various discussions of individual functions.

We can create and own a JSON object by using one of:

- [json_object_new_object()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a68c383f54544fca19b5f2425be397600)
- [json_object* json_object_from_file()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__util_8h.html#a03119ec0a71af4eee95318e9b2aaf05b)
- [json_object* json_object_from_fd()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__util_8h.html#a5b72bf6f3ac8fb03da38d2e2d1e18d1b)
- [json_object* json_object_from_fd_ex()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__util_8h.html#a88c5c7ce735d95f6c3c81c73475e14aa)
- [json_object* json_tokener_parse()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__tokener_8h.html#a236ef64d079822a4411d13eae7190c4d)
- [json_object* json_tokener_parse_ex()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__tokener_8h.html#a61679f178111963a9ffa3c8179553f7a)
- [json_object* json_tokener_parse_verbose()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__tokener_8h.html#a735f2dc755d57ed5c5b807aaaaef3b14)

Now perhaps the most important point, the function _*json_object_get()*_ is the only function that increments the reference count:

- [void json_object_get(json_object \* obj)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a675aa3a9cced685dbfd1c1a770a0c3e4)

From the documentation:

> Increment the reference count of json_object, thereby grabbing shared ownership of obj. 

Recall the function _*json_object_put()*_ decrements the reference count of a JSON object:

- [int json_object_put(json_object \*obj)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#afabf61f932cd64a4122ca8092452eed5)

Again from the documentation:

> Decrement the reference count of json_object and free if it reaches zero.
> You must have ownership of obj prior to doing this or you will cause an imbalance in the reference count.
>
> An obj of NULL may be passed; in that case this call is a no-op.
>
> Returns 1 if the object was freed.

In addition to json_object_put, we can also delete an attribute/value pair within a JSON object:

- [void json_object_object_del(json_object * obj, const char *key)](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#ac6605fdafca20bd5d33c84f4f80a3bda)

> Deletes the given attribute/value pair within json_object *obj for the attribute key if it exist. 
> 
> The reference count will be decremented for the deleted object. If there are no more owners of the value represented by this key, then the value is freed. Otherwise, the reference to the value will remain in memory.

And likewise we can do the same for one or more json objects within a JSON array:

- [void json_object_array_del_idx(json_object *obj, size_t idx, size_t count) ](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a722eca9f578704d3af38b97549242c1f)

> Delete count number of elements from a specified index, idx, in a JSON array. And as expected the reference count will be decremented for each of the deleted objects. If there are no more owners of an element that is being deleted, then the value is freed. Otherwise, the reference to the value will remain in memory.
> 
>When one creates a new JSON object, its reference count is 1 and the caller of this object initially owns it. There is no need to call json_object_get on a newly created JSON object unless you lose ownership of the object but want to maintain ownership for some latter usage.

You may think that for every json_object_get there should eventually follow a json_object_put, much like the classic c way of following an malloc with a free at some latter point. However, not only is this **not always the case** it is **not usually the case**.

How exactly does one transfer ownership of a JSON object?

By using any of the following functions:

- [json_object_object_add()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a27bd808a022251059a43f1f6370441cd)
- [json_object_object_add_ex()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a57d3e444dd7db6b4510d21bf3716a002)
- [json_object_array_add()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a18cdd9a7455e09f36cdf6e5756b7f586)

>No reference counts are changed for the above functions, but the ownership is transferred.

But ownership is not affected by the following functions, nor are reference counts changed:
    
- [json_object_object_get()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a1a097805abb53b4c8a60d573730a8939) or [json_object_object_get_ex()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a90d5f16d58636f01d2ed1a6030c7366a)
- [json_object_array_put_idx()](https://json-c.github.io/json-c/json-c-0.15/doc/html/json__object_8h.html#a1ac0ccdbc13a25da7d8b2dc9e421dfad)

As previously [noted](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/File.md), most of sample programs will follow this template:

|  Pseudo-Code                            | Ownership                                                                                         |
| :-------------------------------------- | :------------------------------------------------------------------------------------------- |
| Create or initialize a root JSON_object |  _*at this point we own the object root and roots reference count is 1*_ |
| Process JSON, saving or displaying JSON data as needed | _*we usually create or initialize other json objects here and transfer ownership of them to root. As longs as roots ownership is not transfered or shared with another object, roots reference count is unchanged*_ |
| Dereference our JSON object |  _*at this point we lose ownership of root and its reference count is decremented. If roots reference count is 0, roots memory and all objects owned by root with a reference count of 0 is freed*_ |


This is the [same point](https://github.com/json-c/json-c/issues/642#issuecomment-656326298) made by [Pierce Lopez](https://github.com/ploxiln), only his answer is more detailed. Read it carefully and make sure you understand it:

> Typically, you create a tree of json objects, either by parsing text with a tokener, or by creating-and-adding them  one-by-one. Typically, every object in the tree will have one reference, from it's parent. When you are done with the tree of objects, you put() just the root object, and that put()s its child objects automatically, and that frees the entire tree in a cascading manner from a single put(). This is pretty simple. You can get a reference to a single child and copy its value in just a few lines, while obviously not freeing the parent, without dealing with any reference count changes, and this is safe.
> 
> You can take a reference to a sub-section of the object tree and use it for a non-trivial amount of time, perhaps after the original object tree was freed, by get()ing root of the sub-tree to increase it's "reference count" ... then you can treat it as an independent object tree. But you may not have to do this, depending on your application.
    
It is highly recommended you use Valgrind to check for memory errors, especially if you are somewhat confused  by the above discussion.

In our next section, we will consider some concrete code examples to illustrate some of the above.
