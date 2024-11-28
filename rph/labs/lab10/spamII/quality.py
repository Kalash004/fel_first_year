def quality_score(tp, tn, fp, fn):
    q = (tp + tn) / (tp + tn + 10 * fp + fn)
    return q

def compute_quality_for_corpus(corpus_dir):
    