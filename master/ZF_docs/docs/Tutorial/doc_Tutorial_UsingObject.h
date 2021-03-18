/**
 * @page DocTag_Tutorial_UsingObject Using object
 *
 * like many other application framework,
 * ZFFramework use single inheritance,
 * most of elements inherit from ZFObject\n
 * and, we use retain count logic to manage memory,
 * just like Object-C\n
 * to create an object, you must use one of alloc function
 * supplied by ZFFramework:
 * @code
 *   // all object can only be allocated in heap,
 *   // so always use pointer here
 *   ZFObject *obj;
 *
 *   // use zfnull to stands for null object,
 *   // which typically same as NULL in C/C++ world
 *   obj = zfnull;
 *
 *   // allocate an object,
 *   // the newly allocated object would have 1 as retain count
 *   obj = zfAlloc(ZFObject);
 *
 *   // retain an object,
 *   // which would increase the object's retain count by 1,
 *   // would be 2 here in this example
 *   zfRetain(obj);
 *
 *   // release an object,
 *   // which would decrease the object's retain count by 1,
 *   // would be 1 here in this example
 *   zfRelease(obj);
 *
 *   // continue to release the object,
 *   // decreasing the retain count to 0,
 *   // and the object would be deleted finally
 *   zfRelease(obj);
 *
 *   // after the object being deleted,
 *   // set it to null is recommended,
 *   // just like it was in the C/C++ world
 *   obj = zfnull;
 * @endcode
 * \n
 * once allocated, you may use object's member method
 * just like normal C++ object:
 * @code
 *   obj->func();
 * @endcode
 *
 * @section DocTag_Tutorial_UsingObject_Advanced Advanced
 * tired of writing retain and release everywhere?
 * we also supply some utility methods to make it easier
 * to manage allocated object:
 * @code
 *   {
 *       // similar to shared_ptr in C++ world
 *       zfautoObject obj = zflineAlloc(MyObject);
 *   } // obj would be released automatically after this code block
 *
 *   {
 *       // allocate an object which looks like being allocated in stack
 *       zfblockedAlloc(MyObject, obj);
 *   } // obj would be released automatically after this code block
 *
 *   // allocate an temp object which would be released automatically
 *   // after end of this code line
 *   func(zflineAlloc(MyObject));
 *
 *   MyObject *obj = zfAlloc(MyObject);
 *   // release the object automatically after end of this code line
 *   func(zflineRelease(obj));
 *
 *   obj = zfAlloc(MyObject);
 *   // most powerful auto release logic similar to autorelease in Object-C
 *   // however, requires ZFThread,
 *   // and may affect performance for a little
 *   func(zfautoRelease(obj)); // obj would be released at future
 * @endcode
 * \n
 * it's recommended to use #zfautoObject for API design
 * when your function needs to return an allocated object
 * @code
 *   zfautoObject myFunc(void)
 *   {
 *       zfblockedAlloc(MyObject, obj);
 *       return obj;
 *   }
 * @endcode
 */

