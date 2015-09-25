package io.branch.referral;

/**
 * Returns a general error if the server back-end is down.
 */
public class BranchError {

    String errorMessage_ = "";

    /* Error processing request since session not initialised yet. */
    public static final int ERR_NO_SESSION = -101;
    /* Error processing request since app doesn't have internet permission. */
    public static final int ERR_NO_INTERNET_PERMISSION = -102;
    /* Error processing request since referral code provided is invalid. */
    public static final int ERR_INVALID_REFERRAL_CODE = -103;
    /* Error processing request since Branch is not initialised. */
    public static final int ERR_BRANCH_INIT_FAILED = -104;
    /* Error processing request since alias is already used. */
    public static final int ERR_BRANCH_DUPLICATE_URL = -105;
    /* Error processing request since alias is already used. */
    public static final int ERR_BRANCH_DUPLICATE_REFERRAL_CODE = -106;
    /* Error redeeming rewards. */
    public static final int ERR_BRANCH_REDEEM_REWARD = -107;


    /**
     * <p>Returns the message explaining the error.</p>
     *
     * @return A {@link String} value that can be used in error logging or for dialog display
     * to the user.
     */
    public String getMessage() {
        return errorMessage_;
    }

    /**
     * <p>Overridden toString method for this object; returns the error message rather than the
     * object's address.</p>
     *
     * @return A {@link String} value representing the object's current state.
     */
    @Override
    public String toString() {
        return getMessage();
    }

    public BranchError(String failMsg, int statusCode) {
        errorMessage_ = failMsg + getLocalisedErrorMessage(statusCode);
    }

    /*
     * <p> Provides localised error messages for the gives status code </p>
     *
     * @param status Http error code or Branch error codes
     *
     * @return A {@link String} with localised error message for the given status
     */
    private String getLocalisedErrorMessage(int statusCode) {
        String errMsg = "";
        if (statusCode == RemoteInterface.NO_CONNECTIVITY_STATUS) {
            errMsg = " Branch API Error: poor network connectivity. Please try again later.";
        } else if (statusCode == RemoteInterface.NO_BRANCH_KEY_STATUS) {
            errMsg = " Branch API Error: Please enter your branch_key in your project's manifest file first.";
        } else if (statusCode == ERR_BRANCH_INIT_FAILED) {
            errMsg = " Did you forget to call init? Make sure you init the session before making Branch calls.";
        } else if (statusCode == ERR_NO_SESSION) {
            errMsg = " Unable to initialize Branch. Check network connectivity or that your branch key is valid.";
        } else if (statusCode == ERR_NO_INTERNET_PERMISSION) {
            errMsg = " Please add 'android.permission.INTERNET' in your applications manifest file.";
        } else if (statusCode == ERR_BRANCH_DUPLICATE_URL) {
            errMsg = " Unable to create a URL with that alias. If you want to reuse the alias, make sure to submit the same properties for all arguments and that the user is the same owner.";
        } else if (statusCode == ERR_BRANCH_DUPLICATE_REFERRAL_CODE) {
            errMsg = " That Branch referral code is already in use.";
        } else if (statusCode == ERR_BRANCH_REDEEM_REWARD) {
            errMsg = " Unable to redeem rewards. Please make sure you have credits available to redeem.";
        } else if (statusCode >= 500) {
            errMsg = " Unable to reach the Branch servers, please try again shortly.";
        } else if (statusCode == 409) {
            errMsg = " A resource with this identifier already exists.";
        } else if (statusCode > 400) {
            errMsg = " The request was invalid.";
        } else {
            errMsg = " Check network connectivity and that you properly initialized.";
        }
        return errMsg;
    }

}
