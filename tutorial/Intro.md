# Intro

Love it or hate it, JSON (JavaScript Object Notation) is ubiquitous these days as a file format. As its name implies, it was derived from JavaScript. Most programming languages provide support for it, either in the standard libraries or via third party libraries. JSON is simply a way of storing or exchanging data for use by a program in a human-readable text file. It is commonly used as a data format for transmitting data over the internet, as an ad hoc application configuration language and _*more*_. As a text format, JSON is completely language independent and uses conventions that are familiar to programmers of the C-family of languages. As such the data representation has a style similar to C structures. The data representation consists of name-value pairs (_*fields*_) and array data types.

The following comment from [json.org](https://www.json.org/json-en.html) should clarify the concept of the JSON format for most programmers:

> JSON is built on two structures:
>
> - A collection of name/value pairs. In various languages, this is realized as an object, record, struct, dictionary, hash table, keyed list, or associative array.
> - An ordered list of values. In most languages, this is realized as an array, vector, list, or sequence.

In this tutorial, we are going to take a meandering path thru learning the [json-c](https://github.com/json-c/json-c) API. We will use functions as needed and perhaps not always do things the 'proper' way at first or at all. We will start playing with the code to get just a general sense of it, and then gradually learn more as time goes on. We'll try to keep the examples applicable to real-world problems and not just be toy snippets. It will be assumed you already know C well enough to get by and are comfortable compiling, linking, and running code. All of needed JSON files and program source code discussed in this tutorial can be found in the [tutorial's github repo](https://github.com/rbtylee/tutorial-jsonc).

If you are unfamiliar with JSON, it may be clearer to consider an actual example. The [JSON example below](https://github.com/rbtylee/tutorial-jsonc/blob/master/src/contact.json) (taken from Wikipedia) is a possible representation of a person:

```
{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    }
  ],
  "children": [],
  "spouse": null
}
```

The example above should be readable enough for most people to see what is going on. To make it even more clear below is the same JSON only represented as a tree, similar to the way a file manager lists your directory structure.

!["JSON tree"](https://github.com/rbtylee/tutorial-jsonc/blob/master/Images/json-tree.png)

For more information on JSON read the [Wikipedia article](https://en.wikipedia.org/wiki/JSON) or look over the [state diagrams](https://en.wikipedia.org/wiki/State_diagram) found on [json.org](https://www.json.org/json-en.html) describing the 'language'.  The official standard describing the JSON format is [RFC 7159](https://tools.ietf.org/html/rfc7159).

## Problems

1. Create a JSON file representing three or more people, with each person containing the same attributes as the JSON example given above. Make sure you have a valid JSON file by using a JSON validation web site.

- Create a C _*struct*_ to store the data contained in this JSON representation of a person.

- Explain what is meant by this comment:
>> "The JSON syntax is not a specification of a complete data interchange. Meaningful data interchange requires agreement between a producer and consumer on the semantics attached to a particular use of the JSON syntax. What JSON does provide is the syntactic framework to which such semantics can be attached." - [The JSON Data Interchange Syntax](https://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)

- Do you have any programs installed that use JSON as a configuration file? If so examine the file.

- What are some potential downsides of using JSON as a configuration file?

- Do some research and find some websites with JSON  as REST API responses.

- Take a look at [jq](https://stedolan.github.io/jq/). Is this a tool you could use?

- One of the more interesting uses of JSON is [Vega](https://vega.github.io/vega/): 
> Vega is a visualization grammar, a declarative language for creating, saving, and sharing interactive visualization designs. With Vega, you can describe the visual appearance and interactive behavior of a visualization in a JSON format, and generate web-based views using Canvas or SVG.

Examine a few of the examples of try to implement one of them using python or javascript. 
