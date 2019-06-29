#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main()
{
    xmlDocPtr pdoc = NULL;
    xmlNodePtr proot_node = NULL,pnode = NULL,pnode1 = NULL;

    pdoc = xmlNewDoc (BAD_CAST "1.0");
    proot_node = xmlNewNode (NULL, BAD_CAST "根节点");
    xmlNewProp (proot_node, BAD_CAST "版本", BAD_CAST "1.0");
    xmlDocSetRootElement (pdoc, proot_node);

    pnode = xmlNewNode (NULL, BAD_CAST "子节点1");

    xmlNewChild (pnode, NULL, BAD_CAST "子子节点1", BAD_CAST "信息");

    xmlAddChild (proot_node, pnode);

    pnode1 = xmlNewNode (NULL, BAD_CAST "子子节点1");  
    xmlAddChild (pnode, pnode1);    
    xmlAddChild (pnode1,xmlNewText (BAD_CAST "这是更低的节点,子子子节点1"));
    
    xmlNewTextChild (proot_node, NULL, BAD_CAST "子节点2", BAD_CAST "子节点2的内容");
    xmlNewTextChild (proot_node, NULL, BAD_CAST "子节点3", BAD_CAST "子节点3的内容");

    xmlSaveFormatFileEnc (argc > 1 ? argv[1]:"-", pdoc, "UTF-8", 1);

    xmlFreeDoc (pdoc);
    xmlCleanupParser ();
    xmlMemoryDump ();

    return 0;
}
