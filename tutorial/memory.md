# Memory management, ownership and references
<table width="100%">
<tr>
<td width="20%" align="left"><a href="https://www.google.com/">Previous</a></td>
<td width="60%" align="center"><a href="https://www.google.com/">Index</a></td>
<td width="20%" align="right" "><a href="https://www.google.com/">Next</a></td>
</tr>
</table>

The json-c library makes memory management somewhat easier than traditional C as a JSON object is owned by either the programmer or another JSON object. A reference count for each JSON object is stored and incremented or decremented by certain json-c functions (get and put).  The JSON object's memory will be freed when its reference count reaches zero. Freeing a JSON object also frees the memory of all the objects the JSON object owns. So in addition to reference counts, A JSON object is either owned by the programmer or another JSON object. Certain json-c functions change the ownership of an object. If more than one JSON object owns a given JSON object, then that object's reference count is a count of all the objects which own it. The only objects programmers need to explicitly manage the memory of in json-c are the JSON objects they own.

This entire discussion is to clarify these issues and answer the question of when to call json_object_put or json_object_get. This issue can be somewhat confusing to new users of the json-c library. Currently, there is no centralized discussion of this in the json-c documentation, instead, the issue is mentioned in various discussions of individual functions.

We can create and own a JSON object by using one of:

- json_object_new_object() 
- json_object* json_object_from_file()
- json_object* json_object_from_fd()
- json_object* json_object_from_fd_ex()
- json_object* json_tokener_parse()
- json_object* json_tokener_parse_ex()
- json_object* json_tokener_parse_verbose()

Now perhaps the most important point, the function _*json_object_get()*_ is the only function that increments the reference count:

- void json_object_get(json_object \* obj)

From the documentation:

> Increment the reference count of json_object, thereby grabbing shared ownership of obj. 

Recall the function _*json_object_put()*_ decrements the reference count of a JSON object:

- int json_object_put(json_object \*obj)

Again from the documentation:

> Decrement the reference count of json_object and free if it reaches zero.
> You must have ownership of obj prior to doing this or you will cause an imbalance in the reference count.
>
> An obj of NULL may be passed; in that case this call is a no-op.
>
> Returns 1 if the object was freed.

In addition to json_object_put, we can also delete an attribute/value pair within a JSON object:

- void json_object_object_del(json_object * obj, const char *key)

> Deletes the given attribute/value pair within json_object *obj for the attribute key if it exist. 
> 
> The reference count will be decremented for the deleted object. If there are no more owners of the value represented by this key, then the value is freed. Otherwise, the reference to the value will remain in memory.

And likewise we can do the same for one or more json objects within a JSON array:

- void json_object_array_del_idx(json_object *obj, size_t idx, size_t count) 

> Delete count number of elements from a specified index, idx, in a JSON array. And as expected the reference count will be decremented for each of the deleted objects. If there are no more owners of an element that is being deleted, then the value is freed. Otherwise, the reference to the value will remain in memory.
> 
>When one creates a new JSON object, its reference count is 1 and the caller of this object initially owns it. There is no need to call json_object_get on a newly created JSON object unless you lose ownership of the object but want to maintain ownership for some latter usage.

You may think that for every json_object_get there should eventually follow a json_object_put, much like the classic c way of following an malloc with a free at some latter point. However, **we will see this is not always the case**.

How exactly does one transfer ownership of a JSON object?

By using any of the following functions:

- json_object_object_add()
- json_object_object_add_ex()
- json_object_array_add() 

>No reference counts are changed for the above functions, but the ownership is transferred.

But ownership is not affected by the following functions, nor are reference counts changed:
    
- json_object_object_get() or json_object_object_get_ex()
- json_object_array_put_idx()

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
