# Parsing a Json object - part 6: Iteration through a JSON obj

We introduced the function _*json_object_object_foreach*_ previously. However json-c offers an alternative 


json-c forces clients to use its private data structures for JSON Object iteration. This API corrects that by abstracting the private json-c details.
