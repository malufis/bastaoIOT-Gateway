import sys
print("Python:", sys.version)
try:
    from cryptography import x509
    import cryptography
    print("cryptography OK:", cryptography.__version__)
except Exception as e:
    print("cryptography FAIL:", e)

try:
    import pydantic
    print("pydantic OK:", pydantic.__version__)
except Exception as e:
    print("pydantic FAIL:", e)
