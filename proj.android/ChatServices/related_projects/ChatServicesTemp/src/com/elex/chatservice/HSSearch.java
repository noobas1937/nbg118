package com.elex.chatservice;

import android.util.Log;

import com.elex.chatservice.util.HSTransliterator;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class HSSearch
{
  public static final String TAG = "HelpShiftDebug";
//  private static DoubleMetaphone m = new DoubleMetaphone();
  private static boolean indexing = false;
  private static boolean markDeinit = false;

  public static void init() {
    if (!indexing) {
      Thread indexThread = new Thread(new Runnable() {
        public void run() {
          HSTransliterator.init();
        }
      });
      indexThread.setDaemon(true);
      indexThread.start();
    }
  }

  public static void deinit() {
    if (!indexing)
      HSTransliterator.deinit();
    else
      markDeinit = true;
  }

  public static String stripHtml(String html)
  {
    return html.replaceAll("<[^>]+>", "");
  }

  public static ArrayList<String> generateTokens(String str) {
    ArrayList tokens = new ArrayList();
    Pattern p = Pattern.compile("\\w+");
    Matcher m = p.matcher(str);
    while (m.find()) {
      if (m.group(0).length() > 2) {
        tokens.add(m.group(0));
      }
    }
    return tokens;
  }

  public static ArrayList<String> generateNgrams(int min, int max, String str) {
    ArrayList output = new ArrayList();
    for (int i = min; (i < str.length()) && (i <= max); i++) {
      output.add(str.substring(0, i));
    }
    return output;
  }

  public static ArrayList<HashMap<String, String>> generateVariations(String word) {
    HashSet output = new HashSet();

    output.add(new HashMap()
    {
    });
    ArrayList ngrams = generateNgrams(2, 10, word);
    for (Object ngram : ngrams) {
      output.add(new HashMap()
      {
      });
    }

    output.add(new HashMap()
    {
    });
    return new ArrayList(output);
  }

  public static ArrayList<HashMap<String, String>> generateSearchVariations(String word) {
    HashSet output = new HashSet();

    output.add(new HashMap()
    {
    });
    output.add(new HashMap()
    {
    });
    return new ArrayList(output);
  }

  public static String sanitize(String str)
  {
    String htmlText = stripHtml(str);
    return HSTransliterator.unidecode(htmlText.toLowerCase());
  }

  public static int calcFreq(String type, String token) {
    if (token.length() == 1)
      return 5;
    if (type == "ngram")
      return token.length();
    if (type == "word")
      return 30;
    if (type == "metaphone") {
      return 1;
    }
    return 1;
  }

  public static HashMap<String, Integer> indexDocument(String doc)
  {
//    ArrayList output = new ArrayList();
//    for (String str : generateTokens(sanitize(doc))) {
//      output.addAll(generateVariations(str));
//    }
//
//    HashMap indexDoc = new HashMap();
//
//    for (HashMap tokenMap : output) {
//      String token = (String)tokenMap.get("value");
//      int tokenFreq = 0;
//      if (indexDoc.containsKey(token)) {
//        tokenFreq = ((Integer)indexDoc.get(token)).intValue();
//      }
//      tokenFreq += calcFreq((String)tokenMap.get("type"), token);
//      indexDoc.put(token, Integer.valueOf(tokenFreq));
//    }
//
//    return indexDoc;
    
    ArrayList output = new ArrayList();
    String str;
    for(Iterator i$ = generateTokens(sanitize(doc)).iterator(); i$.hasNext(); output.addAll(generateVariations(str)))
        str = (String)i$.next();

    HashMap indexDoc = new HashMap();
    String token;
    int tokenFreq;
    for(Iterator i$ = output.iterator(); i$.hasNext(); indexDoc.put(token, Integer.valueOf(tokenFreq)))
    {
        HashMap tokenMap = (HashMap)i$.next();
        token = (String)tokenMap.get("value");
        tokenFreq = 0;
        if(indexDoc.containsKey(token))
            tokenFreq = ((Integer)indexDoc.get(token)).intValue();
        tokenFreq += calcFreq((String)tokenMap.get("type"), token);
    }

    return indexDoc;
  }

  public static HashMap indexDocuments(ArrayList<Faq> docs) {
    if (indexing == true) {
      return null;
    }

    if (!HSTransliterator.isLoaded()) {
      HSTransliterator.init();
      markDeinit = true;
    }

    indexing = true;
    Integer totalDocNum = Integer.valueOf(docs.size());
    ArrayList indexedDocs = new ArrayList();
    Integer i = Integer.valueOf(0);
    for (Faq doc : docs) {
      HashMap indexedDoc = new HashMap();
      indexedDoc.put("terms", indexDocument(doc.getTitle() + " " + doc.getBody()));
      indexedDoc.put("id", i + "");
      i = Integer.valueOf(i.intValue() + 1);
      indexedDocs.add(indexedDoc);
    }
    HashMap globalTerms = new HashMap();
//    for (HashMap doc : indexedDocs) {
//      String docId = (String)doc.get("id");
//      terms = (HashMap)doc.get("terms");
//      for (String term : terms.keySet()) {
//        int termFreq = ((Integer)terms.get(term)).intValue();
//        if (globalTerms.containsKey(term)) {
//          HashMap termMap = (HashMap)globalTerms.get(term);
//          Integer maxFreq = (Integer)termMap.get("maxFreq");
//          if (maxFreq == null) {
//            maxFreq = Integer.valueOf(0);
//          }
//          Integer docFreq = (Integer)termMap.get("docFreq");
//          if (docFreq == null)
//            docFreq = Integer.valueOf(0);
//          else {
//            docFreq = Integer.valueOf(docFreq.intValue() + 1);
//          }
//
//          if (maxFreq.intValue() < termFreq) {
//            termMap.put("maxFreq", Integer.valueOf(termFreq));
//          }
//          termMap.put("docFreq", docFreq);
//          globalTerms.put(term, termMap);
//        } else {
//          HashMap termMap = new HashMap();
//          termMap.put("maxFreq", Integer.valueOf(termFreq));
//          termMap.put("docFreq", Integer.valueOf(1));
//          globalTerms.put(term, termMap);
//        }
//      }
//    }
    for(Iterator i$ = indexedDocs.iterator(); i$.hasNext();)
    {
        HashMap doc = (HashMap)i$.next();
        String docId = (String)doc.get("id");
        HashMap terms = (HashMap)doc.get("terms");
        Iterator i$2 = terms.keySet().iterator();
        while(i$2.hasNext()) 
        {
            String term = (String)i$2.next();
            int termFreq = ((Integer)terms.get(term)).intValue();
            if(globalTerms.containsKey(term))
            {
                HashMap termMap = (HashMap)globalTerms.get(term);
                Integer maxFreq = (Integer)termMap.get("maxFreq");
                if(maxFreq == null)
                    maxFreq = Integer.valueOf(0);
                Integer docFreq = (Integer)termMap.get("docFreq");
                if(docFreq == null)
                    docFreq = Integer.valueOf(0);
                else
                    docFreq = Integer.valueOf(docFreq.intValue() + 1);
                if(maxFreq.intValue() < termFreq)
                    termMap.put("maxFreq", Integer.valueOf(termFreq));
                termMap.put("docFreq", docFreq);
                globalTerms.put(term, termMap);
            } else
            {
                HashMap termMap = new HashMap();
                termMap.put("maxFreq", Integer.valueOf(termFreq));
                termMap.put("docFreq", Integer.valueOf(1));
                globalTerms.put(term, termMap);
            }
        }
    }
    
//    HashMap terms;
//    HashMap tfidf = new HashMap();
//
//    for (HashMap doc : indexedDocs) {
//      docId = (String)doc.get("id");
//      terms = (HashMap)doc.get("terms");
//      for (String term : terms.keySet()) {
//        HashMap termMap = (HashMap)tfidf.get(term);
//        if (termMap == null) {
//          termMap = new HashMap();
//        }
//        Integer localTf = (Integer)terms.get(term);
//        HashMap globalTerm = (HashMap)globalTerms.get(term);
//        Integer maxTf = (Integer)globalTerm.get("maxFreq");
//        Integer docTf = (Integer)globalTerm.get("docFreq");
//
//        Double tf = Double.valueOf(localTf.intValue() / maxTf.intValue());
//        Double idf = Double.valueOf(Math.log10(totalDocNum.intValue() / docTf.intValue()));
//        termMap.put(docId, Double.valueOf(tf.doubleValue() * idf.doubleValue()));
//        tfidf.put(term, termMap);
//      }
//    }
//    String docId;
    
    HashMap tfidf = new HashMap();
    for(Iterator i$ = indexedDocs.iterator(); i$.hasNext();)
    {
        HashMap doc = (HashMap)i$.next();
        String docId = (String)doc.get("id");
        HashMap terms = (HashMap)doc.get("terms");
        Iterator i$2 = terms.keySet().iterator();
        while(i$2.hasNext()) 
        {
            String term = (String)i$2.next();
            HashMap termMap = (HashMap)tfidf.get(term);
            if(termMap == null)
                termMap = new HashMap();
            Integer localTf = (Integer)terms.get(term);
            HashMap globalTerm = (HashMap)globalTerms.get(term);
            Integer maxTf = (Integer)globalTerm.get("maxFreq");
            Integer docTf = (Integer)globalTerm.get("docFreq");
            Double tf = Double.valueOf(localTf.intValue() / maxTf.intValue());
            Double idf = Double.valueOf(Math.log10(totalDocNum.intValue() / docTf.intValue()));
            termMap.put(docId, Double.valueOf(tf.doubleValue() * idf.doubleValue()));
            tfidf.put(term, termMap);
        }
    }
    
    Log.d("HelpShiftDebug", tfidf.toString());

    indexing = false;
    if (markDeinit == true) {
      deinit();
      markDeinit = false;
    }
    return tfidf;
  }

  public static ArrayList<String> queryDocs(String query, HashMap tfidf) {
    HashMap docRanks = new HashMap();
    HashSet resultDocSet = null;

    ArrayList terms = new ArrayList();
    for (String term : generateTokens(sanitize(query))) {
      terms.addAll(generateSearchVariations(term));
    }
    if (tfidf != null) {
//      for (HashMap termMap : terms) {
//        String term = (String)termMap.get("value");
//        String type = (String)termMap.get("type");
//        HashMap termDocMap = (HashMap)tfidf.get(term);
//        if ((termDocMap != null) && (termDocMap.keySet().size() > 0)) {
//          for (String docId : termDocMap.keySet()) {
//            Double docTFID = (Double)termDocMap.get(docId);
//            Double docRank = (Double)docRanks.get(docId);
//            Double docContribution = Double.valueOf(docTFID.doubleValue() * calcFreq(type, term));
//            if (docRank != null)
//              docRanks.put(docId, Double.valueOf(docRank.doubleValue() + docContribution.doubleValue()));
//            else {
//              docRanks.put(docId, docContribution);
//            }
//          }
//
//          HashSet termDocSet = new HashSet();
//          termDocSet.addAll(termDocMap.keySet());
//
//          if ((resultDocSet != null) && (!resultDocSet.isEmpty()))
//            resultDocSet.retainAll(termDocSet);
//          else {
//            resultDocSet = new HashSet(termDocSet);
//          }
//        }
//      }

        Iterator i$ = terms.iterator();
        do
        {
            if(!i$.hasNext())
                break;
            HashMap termMap = (HashMap)i$.next();
            String term = (String)termMap.get("value");
            String type = (String)termMap.get("type");
            HashMap termDocMap = (HashMap)tfidf.get(term);
            if(termDocMap != null && termDocMap.keySet().size() > 0)
            {
                for(Iterator i$2 = termDocMap.keySet().iterator(); i$2.hasNext();)
                {
                    String docId = (String)i$2.next();
                    Double docTFID = (Double)termDocMap.get(docId);
                    Double docRank = (Double)docRanks.get(docId);
                    Double docContribution = Double.valueOf(docTFID.doubleValue() * (double)calcFreq(type, term));
                    if(docRank != null)
                        docRanks.put(docId, Double.valueOf(docRank.doubleValue() + docContribution.doubleValue()));
                    else
                        docRanks.put(docId, docContribution);
                }

                HashSet termDocSet = new HashSet();
                termDocSet.addAll(termDocMap.keySet());
                if(resultDocSet != null && !resultDocSet.isEmpty())
                    resultDocSet.retainAll(termDocSet);
                else
                    resultDocSet = new HashSet(termDocSet);
            }
        } while(true);
    }
    if ((resultDocSet == null) || (resultDocSet.isEmpty())) {
      RankComparator docRankComp = new RankComparator(docRanks);
      TreeMap sortedDocRanks = new TreeMap(docRankComp);
      sortedDocRanks.putAll(docRanks);
      return new ArrayList(sortedDocRanks.keySet());
    }if (resultDocSet.size() == 1) {
      return new ArrayList(resultDocSet);
    }
    HashMap resultDocRanks = new HashMap();
    for (Object docId : resultDocSet) {
      resultDocRanks.put(docId, docRanks.get(docId));
    }
    RankComparator docRankComp = new RankComparator(resultDocRanks);
    TreeMap sortedDocRanks = new TreeMap(docRankComp);
    sortedDocRanks.putAll(resultDocRanks);
    return new ArrayList(sortedDocRanks.keySet());
  }
}