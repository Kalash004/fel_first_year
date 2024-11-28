def quality_score(tp, tn, fp, fn):
    q = (tp + tn) / (tp + tn + 10 * fp + fn)
    return q

def compute_quality_for_corpus(corpus_dir):
    import os
    from utils import read_classification_from_file
    from confmat import BinaryConfusionMatrix
    
    folder = corpus_dir
    truth_file = '!truth.txt'
    prediction_file = '!prediction.txt'
    truth_path = os.path.join(folder, truth_file)
    predict_path = os.path.join(folder, prediction_file)
    
    truth_dict = read_classification_from_file(truth_path)
    predict_dict = read_classification_from_file(predict_path)
    SPAM_TAG = 'SPAM'
    HAM_TAG = 'OK'
    BCM = BinaryConfusionMatrix(pos_tag = SPAM_TAG, neg_tag = HAM_TAG)
    BCM.compute_from_dicts(truth_dict, predict_dict)
    return quality_score(BCM.tp, BCM.tn, BCM.fp, BCM.fn)