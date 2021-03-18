/**
 * @page DocTag_Tutorial_UISerialization UI serialization
 *
 * most of UI elements in ZFFramework is also serializable,
 * and it's easy to serialize from and to xml format\n
 * here's a quick example to see the xml output format:
 * @code
 *   zfblockedAlloc(ZFUIView, parent);
 *   zfblockedAlloc(ZFUIButtonBasic, child);
 *   parent->childAdd(child);
 *   child->layoutParam()->sizeParam(ZFUISizeParamFillFill());
 *   child->buttonLabelText("button");
 *
 *   ZFSerializableData data = ZFObjectToData(parent);
 *   zfstring xmlString;
 *   ZFSerializableDataToXml(ZFOutputForString(xmlString), data);
 *   zfLogTrimT() << "serialized data:";
 *   zfLogTrimT() << xmlString;
 *
 *   ZFSerializableData dataNew = ZFSerializableDataFromXml(ZFInputForBufferUnsafe(xmlString));
 *   zfautoObject objNew = ZFObjectFromData(dataNew);
 *   zfLogTrimT() << "re-serialized object:";
 *   zfLogTrimT() << objNew.toObject();
 * @endcode
 * which would output these:
 * @code
 *   serialized data:
 *   <ZFUIView>
 *       <ZFUIButtonBasic category="child" >
 *           <ZFUITextView name="buttonLabelStyleNormal" >
 *               <zfstring name="text" value="button" />
 *           </ZFUITextView>
 *           <ZFUIViewLayoutParam category="layoutParam" >
 *               <ZFUISizeParam name="sizeParam" value="(Fill, Fill)" />
 *           </ZFUIViewLayoutParam>
 *       </ZFUIButtonBasic>
 *   </ZFUIView>
 *
 *   re-serialized object:
 *   <ZFUIView(0x7fe61e1340b0) (0, 0, 0, 0)>
 * @endcode
 */

